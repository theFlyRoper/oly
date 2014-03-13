/* common.h -- Common functions and header configuration. {{{

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.
   }}} */


#ifndef OLY_COMMON_H
#define OLY_COMMON_H 1

#if HAVE_CONFIG_H
#  include "olyconf.h"
#endif
/** HEADERS */

#include <stdio.h>
#include <sys/types.h>

#if STDC_HEADERS
#  include <stdlib.h>
#  include <string.h>
#  include <errno.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif /*STDC_HEADERS*/

#if HAVE_UNISTD_H
#  include <unistd.h>
#endif

#if HAVE_SYS_WAIT_H
#  include <sys/wait.h>
#endif

#include <time.h>

#ifndef CLOCK_REALTIME_COARSE
#define CLOCK_REALTIME CLOCK_REALTIME_COARSE
#endif

/* might as well.  Assert is a valuable function. */
#include <assert.h>

#ifdef HAVE_UNICODE_USTDIO_H
#include <unicode/ustdio.h>
typedef UChar OChar;
typedef UErrorCode LibErrorNum;
#define OFILE UFILE
#endif /* HAVE_UNICODE_USTDIO_H */

#ifdef HAVE_UNICODE_URES_H
#include <unicode/ures.h>
typedef UResourceBundle ResourceData;
#endif /* HAVE_UNICODE_URES_H */

#ifdef HAVE_UNICODE_USTRING_H
#include <unicode/ustring.h>
#endif /* HAVE_UNICODE_USTRING_H */

#ifdef HAVE_UNICODE_UCNV_H
#include <unicode/ucnv.h>
typedef UConverter OlyConverter;
#endif /* HAVE_UNICODE_UCNV_H */

#ifdef HAVE_UNICODE_UMSG_H
#include <unicode/umsg.h>
#endif /* HAVE_UNICODE_UMSG_H */

#ifdef HAVE_UNICODE_USPREP_H
#include <unicode/usprep.h>
#endif /* HAVE_UNICODE_USPREP_H */

#ifndef WIFEXITED
#  define WIFEXITED(stat)       (((stat) & 0xff) == 0)
#endif
#ifndef WEXITSTATUS
#  define WEXITSTATUS(stat)     ((unsigned)(stat) >> 8)
#endif
#ifndef WIFSTOPPED
#  define WIFSTOPPED(stat)      (((stat) & 0xff) == 0x7f)
#endif
#ifndef WSTOPSIG
#  define WSTOPSIG(stat)        WEXITSTATUS(stat)
#endif
#ifndef WIFSIGNALED
#  define WIFSIGNALED(stat)     (!WIFEXITED(stat) && !WIFSTOPPED(stat))
#endif
#ifndef WTERMSIG
#  define WTERMSIG(stat)        ((stat) & 0x7f)
#endif

#ifndef errno /* Some systems #define this! */
extern int errno;
#endif

#ifndef BUFSIZ
#define BUFSIZ 8192
#endif

/** FUNCTIONS AND SYSTEM MACROS  */

#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS  0
#  define EXIT_FAILURE  1
#endif

#if !HAVE_BZERO && HAVE_MEMSET
#  define bzero(buf, bytes)      ((void) memset (buf, 0, bytes))
#endif

#if !HAVE_STRCHR
#  define strchr index
#endif

#if !HAVE_STRRCHR
#  define strrchr rindex
#endif


/** GNUC, STDC AND CXX */

#ifdef __cplusplus
#  define BEGIN_C_DECLS         extern "C" {
#  define END_C_DECLS           }
#else
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#endif



#ifdef __GNUC__
#  ifndef const
#    define const       __const
#  endif
#  ifndef signed
#    define signed      __signed
#  endif
#  ifndef volatile
#    define volatile    __volatile
#  endif
#else
#  ifdef __STDC__
#    undef  signed
#    define signed
#    undef  volatile
#    define volatile
#  endif
#endif

#ifdef __STDC__
#define STR(x)          #x
#define CONC(x, y)      x##y
#else
#define STR(x)          "x"
#define CONC(x, y)      x/**/y
#endif

/** MALLOC MACROS */
BEGIN_C_DECLS

#define OCALLOC(type, num) \
        ((type *) ocalloc ((num), sizeof(type)))

#define OMALLOC(type, num) \
        ((type *) omalloc ((num) * sizeof(type)))
#define OREALLOC(type, p, num) \
        ((type *) orealloc ((p), (num) * sizeof(type)))
#define OFREE(stale) do { \
        if (stale) { free (stale);  stale = 0; } \
        } while (0)


extern void *ocalloc    (size_t num, size_t size);
extern void *omalloc    (size_t num);
extern void *orealloc   (void *p, size_t num);
extern char *ostrdup    (const char *string);
extern char *ostrcat    (const char *string);
extern char *ostrerror  (int errnum);

#if !HAVE_BASENAME
extern char *basename   (const char *path);
#endif

#if !HAVE_STRLCPY
extern size_t strlcpy (char *dst, const char *src, size_t size);
#endif

#if !HAVE_WCWIDTH
int wcwidth(char ucs);
#endif

#if !HAVE_WCSWIDTH
int wcswidth(char ucs);
#endif

#if !HAVE_STRLCAT
extern size_t strlcat   (char *dst, const char *src, size_t size);
#endif

/*
#if !HAVE_STRCSPN
extern size_t strcspn   (const char *string, const char *accept);
#endif
*/

#if !HAVE_STRERROR
extern char *strerror (int err);
#endif

/*
#if !HAVE_STRSIGNAL
extern char *strsignal  (int signo);
#endif

#if !HAVE_STRSPN
extern size_t strspn    (const char *string, const char *reject);
#endif

#if !HAVE_WAITPID
extern pid_t waitpid    (pid_t pid, int *pstatus, int options);
#endif
*/

END_C_DECLS

#if WITH_DMALLOC
# include <dmalloc.h>
#endif /* WITH_DMALLOC */

#ifdef OLYDEV
#include "oly_dev.h"
#endif /* OLYDEV */

#endif /* !OLY_COMMON_H */

