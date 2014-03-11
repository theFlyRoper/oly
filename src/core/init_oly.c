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

/* pre init functions */
static void *imalloc (size_t num);
static void *icalloc (size_t num, size_t size);
static OlyStatus init_locale (char *locale[], OlyStatus *status);
static char *init_charset (char * preset);
static char *istrdup (const char *string)


/* TODO:
 * This is organized enough right now, but it can be a lot tighter.
 * 1. Consolidate these functions:
 *      new_resource and open_resource = new_resource
 * 2. Move all init functions to static headers and add standard error handler 
 *      functionality to any create function outside initialization.
 * 3. Does not load config right now.  It should, once config is implemented.
 * 4. Proper atexit house cleaning.
 */

Oly *init_oly(char *prog, char *datadir, char *charset, char *locale)
{
    OlyStatus       status = OLY_OKAY;
#ifdef HAVE_UNICODE_USTDIO_H
    UErrorCode       u_status = U_ZERO_ERROR; 
#endif /* HAVE_UNICODE_USTDIO_H */
    OChar           *program_mover = NULL;
    int32_t          len = 0;
    OlyResource     *oly_init_resource = NULL;
    OlyState        *oly_init_state = NULL;
    Oly             *oly_init = (Oly *)xmalloc(sizeof(Oly));
    
    assert(program_name == NULL && prog != NULL && datadir != NULL);
    
    oly->resource_dir = istrdup(datadir);
    oly->data = NULL;
    oly->state = NULL;
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
    len = strlen(basename(prog));
    if (len == 0)
    {
        printf("Program name not found. input: %s, basename: %s. Exiting...\n",
                prog, basename(prog));
        exit(EXIT_FAILURE);
    }
#ifdef HAVE_UNICODE_USTDIO_H
    program_mover = (UChar *)xcalloc(len, sizeof(OChar));
    u_uastrncpy(program_mover, basename(prog), (len-1));
#endif /* HAVE_UNICODE_USTDIO_H */
    program_name = (const OChar *)program_mover;
#ifdef HAVE_UNICODE_USTDIO_H
    /* u_setDataDirectory tells ICU where to look for custom app data.  It is not needed
     * for the internal app data for ICU, which lives in a shared library. 
     */
    u_setDataDirectory(dir);
#endif /* HAVE_UNICODE_USTDIO_H */
    if ( init_locale(&locale, &status) != OLY_OKAY )
    {
        printf("Init: init_locale failed. Err: %i\n", status);
        exit(EXIT_FAILURE);
    }
    if (init_charset (&charset, &status) != OLY_OKAY) 
    {
        printf("Init: init_charset failed. Err: %i\n", status);
        exit(EXIT_FAILURE);
    }

    oly_init_resource = new_resource( locale, charset );

    if ( open_resource(oly_init_resource, datadir, &status) != OLY_OKAY) 
    {
        printf("Init: could not open resource.  %i\n", status);
        exit(EXIT_FAILURE);
    }

    oly_init_state = new_state( oly_init_resource );
    /* u_stderr, u_stdout, u_stdin */
    init_io(locale, charset);
    
    atexit (close_oly);
    
    oly->data           = oly_init_resource;
    oly->state          = oly_init_state;

    return status;
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

OlyStatus init_locale (char *locale[], OlyStatus *status)
{
    int32_t         output_size = 0;
    UErrorCode      u_status  = U_ZERO_ERROR;
    char            *r = NULL;
    
    output_size = uloc_getBaseName(*locale, NULL, NULL, &u_status);
    if (U_FAILURE(u_status)) 
    {
        printf("uloc_getBaseName failed: %s\n",  u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    if ( output_size > 0 )
    {
        r = (char *)icalloc(output_size, sizeof(char));
        output_size = uloc_getBaseName(*locale, r, output_size, &u_status);
        *locale = r;
    } 
    return *status;
}

char *init_charset (char * preset)
{
#ifdef HAVE_UNICODE_UCNV_H
    UErrorCode u_status = U_ZERO_ERROR;
    char *local = ucnv_getAlias( preset, 0, &u_status);
    if (U_FAILURE(u_status)) 
    {
        printf("ucnv_getAlias failed: %s\n",  u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    return local;
#endif /* HAVE_UNICODE_UCNV_H */
}

#ifndef WITH_DMALLOC
char *istrdup (const char *string)
{
  return string ? strcpy (imalloc((strlen (string) + 1)*sizeof(char)), string) : NULL;
}
#endif /* !WITH_DMALLOC */

