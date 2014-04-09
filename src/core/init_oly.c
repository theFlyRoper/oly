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
#include "core/pvt_init_errmsg.h"

/* since resource must have non-i18n error handling
 * before it is available it only makes sense to have a 
 * special constructor available here.
 */

/* pre init functions.  None of these should be called anywhere else, which is why they are all static. */

/* get_home, cleanenv, open_devnull and clean_io_open
 * are adapted from the Secure Programming 
 * Cookbook, John Viega and Matt Messier.
 * 2003, O'Reilly Press, 
 * ISBN: 0-596-00394-3
 */

/* verifies we can open /dev/null, just in case some malicious user has 
 * hijacked all the file streams in order to capture stdout or stderr. */
static int   open_devnull(int fd);
/* gets the user's home directory in a reliable way. */
static char *get_home (struct passwd *pwd);
/* closes any nonstandard files that are open now, then reopens the std* set
 * if needed. */
static void clean_io_open(void) ;
/* cleans the local program environment, removing any environment variables that
 * are not used by Oly and providing secure ones to Oly through more reliable 
 * means. */
static OlyStatus cleanenv (void);
/* collects the startup locale from the environment or system. */
static char *init_locale (const char *locale);
/* collects the startup character set / encoding / codepoint from the env or system 
 * Note that if the U_CHARSET_IS_UTF8 C preprocessor marker is defined,
 * ICU will always return UTF-8 as the default character set. */
static char *init_encoding (const char * preset);
/* Initializes the ICU resource and sets up OChar versions
 * of the locale, encoding and datadir. */
static OlyResource *init_primary_resource(const char *locale, 
        const char *encoding, const char *datadir);
/* initializes ICU IO */
static void init_io(const char *locale, const char *codepage);
/* cleans up after Oly.  May need work. */
static void close_oly(void);
/* pre-init, non-i18n malloc wrapper with out-of-memory checks. */
static void *imalloc (size_t num);
/* pre-init, non-i18n calloc wrapper with out-of-memory checks. */
static void *icalloc (size_t num, size_t size);
/* pre-init, non-i18n strdup wrapper with out-of-memory checks. */
static char *istrdup (const char *string);

/* TODO:
 * This is organized enough right now, but it can be a lot tighter.
 * 1. Does not load config right now.  It should, once config is implemented.
 */

OlyStatus init_oly(const char *prog, 
        const char *datadir, const char *encoding, const char *locale, Oly **oly_out)
{
#ifdef HAVE_UNICODE_USTDIO_H
    UErrorCode       u_status = U_ZERO_ERROR; 
#endif /* HAVE_UNICODE_USTDIO_H */
    OChar           *transfer = NULL;
    char            *inner_encoding, *inner_locale, 
                    *inner_prog     = istrdup(prog),
                    *inner_datadir  = istrdup(datadir)
                    ;
    int32_t          len = 0;
    Oly             *oly_init = (Oly *)imalloc(sizeof(Oly));
    
    assert(inner_prog != NULL && inner_datadir != NULL);
    
    oly_init->program_name = NULL;
    oly_init->resource_dir = NULL;
    oly_init->data = NULL;
    oly_init->state = NULL;
    oly_init->config = NULL;
    oly_init->inbound = NULL;
    oly_init->node_queue = NULL;
    oly_init->outbound = NULL;
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
    len = (strlen(basename(inner_prog))+1);
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
    len = (strlen(inner_datadir)+1);
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
    inner_encoding   = init_encoding(encoding);
    init_io(inner_locale, inner_encoding);

    /* the ICU constructors take char arguments, which is
     * what init_locale and init_encoding provide. */
    oly_init->data = init_primary_resource( inner_locale, 
            inner_encoding, inner_datadir );

    init_errmsg(oly_init);
    
    oly_init->state         = new_state( oly_init->data );
    oly_init->status        = OLY_OKAY;
    oly_init->inbound       = NULL;
    oly_init->outbound      = NULL;
    oly_init->status        = open_node_queue(&(oly_init->node_queue));
    HANDLE_STATUS_AND_DIE(oly_init->status);
    (*oly_out)              = oly_init;
    atexit(close_oly);

    return oly_init->status;
}

void *
imalloc (size_t num)
{
    void *new = malloc (num);
    if (!new)
    {
        fprintf(stderr, "Memory exhausted: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
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

char *
init_locale (const char *locale)
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

char *
init_encoding (const char * preset)
{
#ifdef HAVE_UNICODE_UCNV_H
    /* Currently, Oly builds with U_CHARSET_IS_UTF8, which means
     * this function always returns UTF-8 for default encoding.
     * This is good in that it simplifies use, but bad in that
     * when built on a system where the default encoding is not
     * utf-8, we will have to check the encoding of all inbound
     * files.
     *
     * Cross that bridge later.
     */
    const char *local;
    if (preset == NULL)
    {
        local = ucnv_getDefaultName();
    }
    else 
    {
        local = preset;
    }
    if ( local == NULL ) 
    {
        printf("Init: No encoding found for ucnv_getDefaultName.\n");
        exit(EXIT_FAILURE);
    }
    return (char *)local;
#endif /* HAVE_UNICODE_UCNV_H */
}

OlyResource *
init_primary_resource(const char *locale, const char *encoding, 
        const char *datadir)
{
    OlyResource *res = (OlyResource *)imalloc(sizeof(OlyResource));
    OChar       *n = (OChar *)icalloc(strlen(locale), sizeof(OChar));
#ifdef HAVE_UNICODE_URES_H
    UErrorCode      u_status  = U_ZERO_ERROR;
    res->locale  = u_uastrcpy(n, locale);
    n = (OChar *)icalloc(strlen(encoding), sizeof(OChar));
    res->encoding = u_uastrcpy(n, encoding);
    res->resource = (ResourceData *)ures_open(datadir, locale, &u_status);
    if (U_FAILURE(u_status)) 
    {
        printf("Resource file error. Status: %s.\n",
                u_errorName(u_status));
    }
#endif /* HAVE_UNICODE_URES_H */
    return res; 
}

static void 
close_oly (void) 
{
    void *free_me;
    close_resource(oly->data);

    if (oly->program_name != NULL)
    {
        free_me = (void *)oly->program_name; 
        OFREE(free_me); }
    if (oly->resource_dir != NULL)
    {
        free_me = (void *)oly->resource_dir; 
        OFREE(free_me);
    }
    if (oly->config != NULL)
    {
        free_me = (void *)oly->config; 
        OFREE(free_me);
    }
    u_fclose (u_stdout);
    u_fclose (u_stdin);
}

static void
init_io(const char *locale, const char *codepage) 
{
    char    err_buffer[BUFSIZ];
    int     errnum=0;

    assert((u_stderr == NULL) && (u_stdin == NULL) && (u_stdout == NULL));
    u_stderr=u_finit(stderr, locale,  codepage);
    if(!u_stderr) {
        if ( strerror_r( errnum, err_buffer, BUFSIZ ) == 0 ) {
            printf("Err: u_stderr, %s, locale: %s, codepage %s\n", err_buffer, locale, codepage);

        }
        exit(1);
    }
    u_stdout = u_finit(stdout, locale, codepage);
    if(!u_stdout) {
        if ( strerror_r( errnum, err_buffer, BUFSIZ ) == 0 ) {
            printf("Err: u_stdout, %s\n", err_buffer);
        }
        exit(1);
    }
    u_stdin = u_finit(stdin, locale, codepage);
    if(!u_stdin) {
        if ( strerror_r( errnum, err_buffer, BUFSIZ ) == 0 ) {
            printf("Err: u_stdin, %s\n", err_buffer);
        }
        exit(1);
    }
}

/* get_home, cleanenv 
 * are adapted from the Secure Programming 
 * Cookbook, John Viega and Matt Messier.
 * 2003, O'Reilly Press, 
 * ISBN: 0-596-00394-3
 */

char *
get_home (struct passwd *pwd) 
{
    char *home = (char *)imalloc((strlen(pwd->pw_dir)) + sizeof("HOME=") - 1);
    strcpy(home, "HOME=");
    strcat(home, pwd->pw_dir);
    return home;
}

/* These arrays are both null-terminated. 
 * was thinking of recording current user, but, enh.
 */

const char *
spc_restricted_environ[  ] = {
    0,            /* spot for home. */
    "IFS= \t\n",
    "PATH=" _PATH_STDPATH,
    0
};
 
const char *
spc_preserve_environ[  ] = {
    "LANGUAGE",
    "LC_MESSAGES",
    "LC_CTYPE",
    "LC_ALL",
    "LANG",
    "TZ",
    /* ./tests/runtests provides these two environment variables so 
     * it behooves us to not clobber them during tests. :) */
#ifdef TESTING
    "SOURCE",
    "BUILD",
#endif /* TESTING */
    0
};


OlyStatus
cleanenv (void) {
    int         i,status=OLY_OKAY;
    char        **new_environ, *ptr;
    const char  *value, *var; 
    size_t arr_size = 1, arr_ptr = 0, len = 0, 
           new_size = 0;
 
    uid_t           uid = getuid();
    struct passwd   *pwd;
    
    if ( ! (pwd = getpwuid(uid)) ){
        endpwent();
        status=OLY_ERR_NOPWD;
        return status;
    }

    spc_restricted_environ[0] = get_home(pwd);
    endpwent();
  
    for (i = 0;  (var = spc_restricted_environ[i]) != 0;  i++)
    {
        new_size += strlen(var) + 1;
        arr_size++;
    }

    for (i = 0;  (var = spc_preserve_environ[i]) != 0;  i++)
    {
        if (!(value = getenv(var))) continue;
        new_size += strlen(var) + strlen(value) + 2;
        arr_size++;
    }

    new_size += (arr_size * sizeof(char *));
    if (!(new_environ = (char **)imalloc(new_size))) 
        abort(  );
    new_environ[arr_size - 1] = 0;
    
    ptr = (char *)new_environ + (arr_size * sizeof(char *));
    for (i = 0;  (var = spc_restricted_environ[i]) != 0;  i++) 
    {
        new_environ[arr_ptr++] = ptr;
        len = strlen(var);
        memcpy(ptr, var, len + 1);
        ptr += len + 1;
    }
    for (i = 0;  (var = spc_preserve_environ[i]) != 0;  i++) {
        if (!(value = getenv(var))) continue;

        new_environ[arr_ptr++] = ptr;
        len = strlen(var);
        memcpy(ptr, var, len);
        *(ptr + len) = '=';
        memcpy(ptr + len + 1, value, strlen(value) + 1);
        ptr += len + strlen(value) + 2;
    }
    environ = new_environ;
    return status;
}

/* open_devnull and clean_io_open
 * are adapted from the Secure Programming 
 * Cookbook, John Viega and Matt Messier.
 * 2003, O'Reilly Press, 
 * ISBN: 0-596-00394-3
 */

void 
clean_io_open(void) {
  int           fd, fds;
  struct rlimit lim;
  struct stat   st;
  int           status;
  /* posix provides these macros, so why not use them? */
  int         std_fileno[] = {
                STDIN_FILENO, 
                STDOUT_FILENO,
                STDERR_FILENO };
  /* make sure all open descriptors other than the standard ones are closed */
  status = getrlimit(RLIMIT_NOFILE, &lim);
  if (status != 0) {
      abort();
  }
  if ((fds = (int)lim.rlim_max) == -1) {
#ifndef OPEN_MAX
    fds = sysconf(_SC_OPEN_MAX);
#else
    fds = OPEN_MAX;
#endif
  }
  for (fd = 3; fd < fds; fd++) {
    close(fd);
  }
  /* Verify std descriptors are open.  If no, attempt open with dev/null.
   * if unsuccessful, abort.
   */
  for (fd = 0; fd < 3; fd++) {
    if (fstat(std_fileno[fd], &st) == -1 && 
        (errno != EBADF || !open_devnull(std_fileno[fd]))) {
      abort();
    }
  }
}

int 
open_devnull(int fd) {
  FILE *f = 0;
  if ( fd == STDIN_FILENO ) {
    f = freopen(_PATH_DEVNULL, "rb", stdin);
  } else if ( fd == STDOUT_FILENO ) {
    f = freopen(_PATH_DEVNULL, "wb", stdout);
  } else if ( fd == STDERR_FILENO ) {
    f = freopen(_PATH_DEVNULL, "wb", stderr);
  }
  return ( f && fileno(f) == fd );
}


