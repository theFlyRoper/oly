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
#include <assert.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <errno.h>
#include <paths.h>
#include <libgen.h>

#include "oly/core.h"

static void         close_oly(void);
static void         init_io(const char *locale, const char *codepage);
static int          open_devnull(int fd);
static void         clean_io_open(void);
static void         set_program_name(char *path);
static oly_status   cleanenv(void);
static oly_status   set_resource_dir(const char *dir, oly_status *status);
static oly_status   init_resource_api(const char *dir, oly_status *status);
static oly_status   get_default_charset (char *charset[], const int32_t len, 
                        oly_status *status);
static oly_status get_default_locale (char *locale[], const int32_t len, 
                        oly_status *status);
static int32_t count_tokens (char *s, char *delims);
static int32_t count_nondelim_chars (char *s, char *delims);
static char **token_str_to_array(char *s, char *delims, oly_status *status); 

oly_status
init_oly(Oly *oly, char *prog) {
    oly_status       status = OLY_OKAY;
#ifdef HAVE_UNICODE_USTDIO_H
    UErrorCode       u_status = U_ZERO_ERROR; 
#endif /* HAVE_UNICODE_USTDIO_H */
    char             buf[BUFSIZ], *loc = NULL, *charset = NULL; 
    ochar           *program_mover = NULL;
    int32_t          len = 0;
    oly_resource    *oly_init_resource = NULL;
    atexit (close_oly);
    assert(program_name == NULL && prog != NULL);
    clean_io_open();
    if ( cleanenv() != OLY_OKAY )
    {
        abort();
    }

    /* program name get. */
    len = (strlen(basename(prog)) * sizeof(ochar));
    if (len == 0)
    {
        printf("Program name not found. input: %s, basename: %s. Exiting...\n"
                prog, basename(prog));
        exit(EXIT_FAILURE);
    }
    program_mover = (ochar *)xcalloc(len);
#ifdef HAVE_UNICODE_USTDIO_H
    program_mover = (ochar *)u_uastrcpy((UChar *)program_mover, prog);
#endif /* HAVE_UNICODE_USTDIO_H */
    program_name = (const ochar *)program_mover;
    if (set_resource_dir(const char *dir, oly_status *status) != OLY_OKAY)
    {
        printf("Could not set resource dir, error: %i\n", status);
        exit(EXIT_FAILURE);
    }
#ifdef HAVE_UNICODE_USTDIO_H
    /* Initialize ICU */
    u_init(&u_status);
    if (U_FAILURE(u_status)) {
        printf("Could not initialize ICU, error: %s. Exiting...\n",
                u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
#endif /* HAVE_UNICODE_USTDIO_H */
    if (get_default_locale (&locale, &status) != OLY_OKAY) 
    {
        printf("Init: get_default_locale failed. Err: %i\n", status);
        exit(EXIT_FAILURE);
    }
#ifdef HAVE_UNICODE_USTDIO_H
    oly_init_resource = (oly_resource *)ures_open(OLY_RESOURCE, locale, &u_status); 
    if (U_FAILURE(u_status)) {
        printf("Main resource file error. Status: %s. Exiting...\n",
                u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
#endif /* HAVE_UNICODE_USTDIO_H */
    
    /*get_default_charset (char *fillin_charset[], const int32_t len, 
                oly_status *status) ; */

    /* u_stderr, u_stdout, u_stdin */
    init_io(locale, NULL);

    oly->messages   = oly_init_resource;
    oly->locale     = locale;
    oly->charset    = charset;
}


oly_status   set_resource_dir(const char *dir, oly_status *status)
{
    *status = OLY_OKAY;
#ifdef HAVE_UNICODE_USTDIO_H
    /* u_setDataDirectory tells ICU where to look for custom app data.  It is not needed
    * for the internal app data for ICU, which lives in a shared library. 
    */
    u_setDataDirectory(dir);
#endif /* HAVE_UNICODE_USTDIO_H */
    return *status;
}
/* open_devnull, get_home, clean_io_open and cleanenv 
 * are adapted from the Secure Programming 
 * Cookbook, John Viega and Matt Messier.
 * 2003, O'Reilly Press, 
 * ISBN: 0-596-00394-3
 */

/* These arrays are both null-terminated. 
 * was thinking of recording current user, but, enh.
 */

char *
oget_home (struct passwd *pwd) {
    char *home = (char *)xmalloc((strlen(pwd->pw_dir)) + sizeof("HOME=") - 1);
    strcpy(home, "HOME=");
    strcat(home, pwd->pw_dir);
    return home;
}

const char *
spc_restricted_environ[  ] = {
    "IFS= \t\n",
    "PATH=" _PATH_STDPATH,
    0,            /* spot for home. */
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
    0
};


oly_status
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
    if (!(new_environ = (char **)xmalloc(new_size))) 
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


#ifndef OPEN_MAX
#define OPEN_MAX 256
#endif /* OPEN_MAX */
#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif /* STDIN_FILENO */
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif /* STDOUT_FILENO */
#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif /* STDERR_FILENO */

int open_devnull(int fd) {
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

void clean_io_open(void) {
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
    fds = OPEN_MAX;
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


void
init_io(const char *locale, const char *codepage) 
{
    char    err_buffer[BUFSIZ];
    int     errnum=0;

    assert((u_stderr == NULL) && (u_stdin == NULL) && (u_stdout == NULL))
    u_stderr=u_finit(stderr, locale,  codepage);
    if(!u_stderr) {
        if ( strerror_r( errnum, err_buffer, BUFSIZ ) == 0 ) {
            printf("%s", err_buffer);
        }
        exit(1);
    }
    u_stdout = u_finit(stdout, locale, codepage);
    if(!u_stdout) {
        if ( strerror_r( errnum, err_buffer, BUFSIZ ) == 0 ) {
            printf("%s", err_buffer);
        }
        exit(1);
    }
    u_stdin = u_finit(stdin, locale, codepage);
    if(!u_stdin) {
        if ( strerror_r( errnum, err_buffer, BUFSIZ ) == 0 ) {
            printf("%s", err_buffer);
        }
        exit(1);
    }
}

void close_oly (void) {
    if (fclose (stdout) != EXIT_SUCCESS) {
        perror ("oly: write error");
        exit (EXIT_FAILURE);
    }
    if (fclose (stdin) != EXIT_SUCCESS) {
        perror ("oly: read error on close");
        exit (EXIT_FAILURE);
    }
}

oly_status get_default_charset (char *charset[], const int32_t len, 
                oly_status *status)
{
    int32_t         output_size = 0;
    UErrorCode      u_status  = U_ZERO_ERROR;
    char            result[OLY_SMALL_BUFFER], *result_ptr;
    *status         = OLY_OKAY;
    
    result_ptr = ucnv_open(NULL, &u_status);
    if (U_FAILURE(u_status)) 
    {
        printf("ucnv_open failed: %s\n",  u_errorName(u_status));
        *status = OLY_ERR_LIB;
    }
    output_size = strlen(result_ptr);
    if (( output_size > 0 ) &&  ( *status == OLY_OKAY ))
    {
        *charset = (char *)xmalloc( (output_size) * ( sizeof(char) ));
        strncpy(*charset, result, output_size);
        *((*charset) + output_size) = '\0';
    } 
    else if (*status == OLY_OKAY) 
    {
            *status = OLY_ERR_INIT;
    }
    return *status;
}

oly_status get_default_locale (char *locale[], const int32_t len, 
                oly_status *status)
{
    /* sorta follows the GNU coding standards; looks at LANGUAGE 
    * (a colon-separated list of language codes) first, then uses
    * ICU's uloc_getName function, which sets locale to the default 
    * if it is null.
    */
    int32_t         output_size = 0, tokens = 0;
    UAcceptResult   acceptable;
    UErrorCode      u_status  = U_ZERO_ERROR;
    char           *language_val = getenv("LANGUAGE"), sep[1] = ":";
    char          **curr = NULL, result[OLY_SMALL_BUFFER];
    *status             = OLY_OKAY;
    UEnumeration    *available;
    available       = ures_openAvailableLocales(OLY_RESOURCE, &u_status);
    while ( language_val != NULL ) 
    {
        if (U_FAILURE(u_status)) 
        {
            *status = OLY_ERR_LIB;
            break;
        }
        curr  = token_str_to_array(language_val, sep, status);
        tokens   = count_tokens (language_val, sep);
        output_size = uloc_acceptLanguage(result, OLY_SMALL_BUFFER, &acceptable, 
            (const char **)curr, tokens, available, &u_status) ;
        language_val = NULL;
        if (U_FAILURE(u_status)) 
        {
            printf("uloc_acceptLanguage failed: %s\n",  u_errorName(u_status));
            *status = OLY_ERR_LIB;
        }
    }
    if ((output_size <= 0) && (*status == OLY_OKAY))
    {
        output_size = uloc_getBaseName(NULL, result, OLY_SMALL_BUFFER, &u_status);
    }
    if (U_FAILURE(u_status)) 
    {
        printf("uloc_getBaseName failed: %s\n",  u_errorName(u_status));
        *status = OLY_ERR_LIB;
    }
    if (( output_size > 0 ) &&  ( *status == OLY_OKAY ))
    {
        *locale = (char *)xmalloc( (output_size + 1) * ( sizeof(char) ));
        strncpy(*locale, result, output_size);
        *((*locale) + output_size) = '\0';
    } 
    else if (*status == OLY_OKAY) 
    {
            *status = OLY_ERR_INIT;
    }
    return *status;
}


/* allocates a char ** array and copies each token into it. 
 * will always return a (char **) where there is at least one string with 
 * an empty value.
 */
char **
token_str_to_array(char *s, char *delims, oly_status *status) 
{
  const int32_t    arr_size = count_tokens(s,delims), 
                  num_chars = count_nondelim_chars(s,delims);
  int32_t          arr_ptr = 0, strsize = 0;
  /* we need arr_size char pointers and arr_size (for the EOLs) + num_chars chars. */
  char          **result = (char **)xmalloc(
                  ((arr_size * sizeof(char *)) 
                   + ((arr_size + num_chars) * sizeof(char)))),
                 *next, *result_position;

  assert(result != NULL);

  *status = OLY_OKAY;
  result_position = (char *)(result + (arr_size * sizeof(char *)));
  if ( delims != NULL ) {
    while ( s != NULL && *(s += strspn(s, delims)) != '\0' ) {
      next = strpbrk(s, delims);
      if (next != NULL) {
        strsize = (next - s);
      } else {
        strsize = strlen(s);
      };
      strncpy(result_position, s, strsize);
      *(result_position + strsize) = '\0';
      result[arr_ptr++] = result_position;
      result_position += (strsize +1);
      s = next;
    }
  }
  return result;
}

int32_t
count_tokens (char *s, char *delims)
{
  int32_t  current_count = 0;

  /* loop over s, adding 1 to current_count for each iteration. */
  if ( delims != NULL ) {
    while ( s != NULL && *(s += strspn(s, delims)) != '\0' ) {
      current_count++;
      s = strpbrk(s, delims);
    }
  }
  return current_count;
}

int32_t
count_nondelim_chars (char *s, char *delims)
{
  int32_t  current_count = 0;
  char    *next;

  /* delims should never be null. */
  assert( delims != NULL ) ;
  /* loop over s, adding the difference between the next and the primary */
  while ( s != NULL && *(s += strspn(s, delims)) != '\0' ) {
    next = strpbrk(s, delims);
    if ( next != NULL ) {
      current_count += ( next - s );
    } else {
      current_count += strlen(s);
    }
    s = next;
  }
  return current_count;
}


