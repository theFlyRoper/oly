/*
 * Takes an oly string and what the string should be, and assumes 
 * the test passes if those strings match (using strcmp).
 *
 * Adapted from the is_string function from c-tap-harness.
 */

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
# include <direct.h>
#else
# include <sys/stat.h>
#endif
#include <sys/types.h>
#include <unistd.h>

#include <tests/tap/basic.h>
#include <tests/tap/is_unicode_string.h>

/* Windows provides mkdir and rmdir under different names. */
#ifdef _WIN32
# define mkdir(p, m) _mkdir(p)
# define rmdir(p)    _rmdir(p)
#endif

static unsigned long _failed  = 0;

#define UPRINT_DESC(prefix, format)              \
    do {                                        \
        if (format != NULL) {                   \
            va_list args;                       \
            if (prefix != NULL)                 \
                fprintf(stdout, "%s", prefix);           \
            va_start(args, format);             \
            u_vfprintf(u_stdout, format, args);              \
            va_end(args);                       \
        }                                       \
    } while (0)

void
u_diag(const char *format, ...)
{
    va_list args;

    u_fflush(u_stderr);
    check_diag_files();
    u_fflush(u_stdout);
    u_fprintf(u_stdout, "# ");
    va_start(args, format);
    u_vfprintf(u_stdout, format, args);
    va_end(args);
    u_fprintf(u_stdout, "\n");
}

UChar *char_to_utf8(const char *input)
{
    int32_t     dest_measured = 0;
    UErrorCode  u_status = U_ZERO_ERROR;
    UChar *str ;
    /* preflight */
    str = (UChar *)bcalloc(strlen(input)+1, sizeof(UChar));
    str = u_strFromUTF8(str, strlen(input), &dest_measured, input, 
            strlen(input), &u_status);
    if ( U_FAILURE(u_status) )
    {
        diag("ICU error: %s", u_errorName(u_status));
        return NULL;
    }
    return str;
}

void
is_unicode_string(const UChar *wanted, const UChar *seen, 
        const char *format, ...)
{
    if (wanted == NULL)
        wanted = char_to_utf8("(null)");
    if (seen == NULL)
        seen = char_to_utf8("(null)");

    u_fflush(u_stderr);
    check_diag_files();
    if (u_strcmp(wanted, seen) == 0)
        u_fprintf(u_stdout, "ok %lu", testnum++);
    else {
        u_diag("wanted: %S", wanted);
        u_diag("  seen: %S", seen);
        u_fprintf(u_stdout, "not ok %lu", testnum++);
        _failed++;
    }
    UPRINT_DESC(" - ", format);
    putchar('\n');
}
