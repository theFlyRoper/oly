/* init_oly.c -- perform initial cleaning of environment and initialize ICU  License GPL2+ {{{
 * Copyright (C) 2012 Oly Project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * }}} */

#include "oly/common.h"

#include <pwd.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <errno.h>
#include <paths.h>
#include <libgen.h>

#include "oly/core.h"
#include "pvt_core.h"
#include "pvt_resources.h"
/* was not planning to include the resource header here
 * but since resource must have non-i18n error handling
 * before it is available it only makes sense to have a 
 * special constructor available here.
 */

/* pre init functions */
static void *imalloc (size_t num);
static void *icalloc (size_t num, size_t size);
static char *istrdup (const char *string);
static char *init_locale (char *locale);
static char *init_charset (char * preset);
static OlyResource *init_primary_resource(const char *locale, 
        const char *charset, const char *datadir);

/* TODO:
 * This is organized enough right now, but it can be a lot tighter.
 * 1. Does not load config right now.  It should, once config is implemented.  Placeholder in the arguments.
 */

Oly *init_oly(const char *prog, 
        const char *datadir, const char *charset, const char *locale)
{
    OlyStatus        status = OLY_OKAY;
#ifdef HAVE_UNICODE_USTDIO_H
    UErrorCode       u_status = U_ZERO_ERROR; 
#endif /* HAVE_UNICODE_USTDIO_H */
    OChar           *transfer = NULL;
    char            *inner_charset, *inner_locale, 
                    *inner_prog =  istrdup(prog),
                    *inner_datadir =  istrdup(datadir)
                    ;
    int32_t          len = 0;
    Oly             *oly_init = (Oly *)imalloc(sizeof(Oly));
    
    assert(inner_prog != NULL && inner_datadir != NULL);
    
    oly_init->program_name = NULL;
    oly_init->resource_dir = NULL;
    oly_init->data = NULL;
    oly_init->state = NULL;
    oly_init->config = NULL;
    /* aborts if encounters unusual states or unclosable files */
    clean_io_open();
    if ( cleanenv() != OLY_OKAY )
    {
        abort();
    }
#ifdef HAVE_UNICODE_USTDIO_H
    /* Initialize ICU */
    u_init(&u_status);
    if (U_FAILURE(u_status)) 
    {
        printf("Init: ICU not found - %s\n",
                u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
#endif /* HAVE_UNICODE_USTDIO_H */
    /* program name get. */
    len = strlen(basename(inner_prog));
    if (len == 0)
    {
        printf("Init: Program name not found. inner_prog = %s basename = %s\n",
                inner_prog, basename(inner_prog));
        exit(EXIT_FAILURE);
    }
#ifdef HAVE_UNICODE_USTDIO_H
    transfer = (UChar *)icalloc(len, sizeof(OChar));
    u_uastrncpy(transfer, basename(inner_prog), (len-1));
#endif /* HAVE_UNICODE_USTDIO_H */
    oly_init->program_name = (const OChar *)transfer;
    /*  attach data directory */
    len = strlen(inner_datadir);
    if (len == 0)
    {
        printf("Init: Resource dir not found. inner_datadir = %s\n",
                inner_datadir);
        exit(EXIT_FAILURE);
    }
#ifdef HAVE_UNICODE_USTDIO_H
    transfer = (UChar *)icalloc(len, sizeof(OChar));
    u_uastrncpy(transfer, inner_datadir, (len-1));
#endif /* HAVE_UNICODE_USTDIO_H */
    oly_init->resource_dir = (const OChar *)transfer;
#ifdef HAVE_UNICODE_USTDIO_H
    /* u_setDataDirectory tells ICU where to look for custom app data.  It is not needed
     * for the internal app data for ICU, which lives in a shared library. 
     */
    u_setDataDirectory(inner_datadir);
#endif /* HAVE_UNICODE_USTDIO_H */

    /* u_stderr, u_stdout, u_stdin */
    inner_locale    = init_locale(locale);
    inner_charset   = init_charset(charset);
    init_io(inner_locale , inner_charset);

    /* the ICU constructors take char arguments, which is
     * what init_locale and init_charset provide. */
    oly_init->data = init_primary_resource( inner_locale, 
            inner_charset, inner_datadir );

    oly_init->state = new_state( oly_init->data );
    
    return oly_init;
}

void *
imalloc (size_t num)
{
    void *new = malloc (num);
    if (!new)
    {
        fprintf(stderr, "Memory exhausted: %s\n", strerror(errno));
    }
    return new;
}

void *
icalloc (size_t num, size_t size)
{
  void *new = imalloc (num * size);
  bzero (new, num * size);
  return new;
}

char *
istrdup (const char *string)
{
  return string ? strcpy(imalloc (strlen (string) + 1), string) : NULL;
}

char *init_locale (char *locale)
{
    int32_t         output_size = BUFSIZ;
    UErrorCode      u_status  = U_ZERO_ERROR;
    char            result[BUFSIZ], *r = NULL;

    r = result;
    
    output_size = uloc_getBaseName(locale, r, output_size, &u_status);
    if (U_FAILURE(u_status)) 
    {
        printf("Init: uloc_getBaseName failed - %s\n",
                u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    if ( output_size > 0 )
    {
        r = (char *)icalloc(output_size, sizeof(char));
        if (!(output_size == uloc_getBaseName(locale, r, output_size, 
                        &u_status)))
        {
            printf("Init: could not confirm locale string size. Err: %s\n",
                    u_errorName(u_status));
            exit(EXIT_FAILURE);
        }
    } 
    return r;
}

char *init_charset (char * preset)
{
#ifdef HAVE_UNICODE_UCNV_H
    UErrorCode u_status = U_ZERO_ERROR;
    /* Currently, Oly builds with U_CHARSET_IS_UTF8, which means
     * this function always returns UTF-8 for default charset.
     * This is good in that it simplifies use, but bad in that
     * when built on a system where the default charset is not
     * utf-8, we will have to check the charset of all inbound
     * files.
     *
     * Cross that bridge later.
     */
    char *local = ucnv_getDefaultName();
    if ( local == NULL ) 
    {
        printf("Init: No encoding found for ucnv_getAlias.\n");
        exit(EXIT_FAILURE);
    }
    return local;
#endif /* HAVE_UNICODE_UCNV_H */
}

OlyResource *
init_primary_resource(const char *locale, const char *charset, 
        const char *datadir)
{
    OlyResource *res = (OlyResource *)imalloc(sizeof(OlyResource));
    OChar       *n = (OChar *)icalloc(strlen(locale), sizeof(OChar));
#ifdef HAVE_UNICODE_URES_H
    UErrorCode      u_status  = U_ZERO_ERROR;
    res->locale  = u_uastrcpy(n, locale);
    n = (OChar *)icalloc(strlen(charset), sizeof(OChar));
    res->charset = u_uastrcpy(n, charset);
    res->resource = (ResourceData *)ures_open(datadir, locale, &u_status);
    if (U_FAILURE(u_status)) 
    {
        printf("Resource file error. Status: %s.\n",
                u_errorName(u_status));
    }
#endif /* HAVE_UNICODE_URES_H */
    return res; 
}


