/* basic_functions.c - core functionality that is used throughout oly. License GPL 2+{{{
 * Copyright (C) 2014 Oly Project
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
#include "sys/types.h"
#include "oly/core.h"
#include "pvt_core.h"

/* count_file_bytes.c - retrieves file size for malloc. */
OlyStatus
count_file_bytes(FILE *file, size_t *file_size)
{
    fpos_t pos;
    OlyStatus status = OLY_OKAY;
    fseek(file, 0, SEEK_END);
    if (fgetpos(file, &pos) != 0) 
    {
        status = OLY_ERR_FILEIO;
    }
    *file_size = (size_t)ftello(file);
    fseek(file, 0, SEEK_SET);
    return status;
}

/* transforms a c string to an OChar string. Always returns 
 * something which is null terminated. o must not be null and buffer_size must be
 * greater than 1.  c may be null.  The function will return an empty string
 * in that case. */
OChar *
cstr_to_ostr(OChar *o, size_t buffer_size, const char *c)
{
    OChar    *superzero = (o + buffer_size);
    assert((o != NULL) && (buffer_size > 1));
    *superzero = '\0';
    if (o == NULL)
    {
        *o = 0x0;
        superzero = o+1;
        *superzero = 0x0;
        return o;
    }
    *superzero = '\0';
#ifdef HAVE_UNICODE_USTDIO_H
    return (OChar *)u_uastrncpy((UChar *)o, c, (buffer_size-1));
#endif /* HAVE_UNICODE_USTDIO_H */
}

/* transforms an OChar string into a c string. Always returns 
 * something which is null terminated. o must not be null and buffer_size must be
 * greater than 1.  c may be null or empty.  The function will return an empty string
 * in that case. */
char  *
ostr_to_cstr(char *c, size_t buffer_size, const OChar *o)
{
    char    *superzero = (c+buffer_size);
    assert((c != NULL) && (buffer_size > 1));
    *superzero = '\0';

    if (o == NULL)
    {
        *c = '\0';
        return c;
    }
#ifdef HAVE_UNICODE_USTDIO_H
    return u_austrncpy(c, (UChar *)o, (buffer_size-1));
#endif /* HAVE_UNICODE_USTDIO_H */
}

/* The resource data encoding for oly serves as the default encoding throughout,
 * since the Oly object is the primary object throughout the program. */

OChar *
get_default_encoding( void )
{
    return get_encoding(oly->data);
}

OChar *
get_default_locale( void )
{
    return get_locale(oly->data);
}

const OChar *
get_program_name( void )
{
    return (const OChar *)oly->program_name;
}

const OChar *
get_resource_dir( void )
{
    return (const OChar *)oly->resource_dir;
}

ResourceData *
get_oly_resource( Oly *oly )
{
    return get_resource_data(oly->data);
}

OlyStatus
get_main_config( OlyConfig **config )
{
    OlyStatus status = OLY_OKAY;
    *config = oly->config;
    return status;
}

const char *char_default_encoding( void )
{
    return ucnv_getDefaultName 	( );
}

const char *char_default_locale( void )
{
    UErrorCode u_status;
    const char * retval = ures_getLocaleByType(get_resource_data(oly->data), 
            ULOC_ACTUAL_LOCALE, &u_status );
    return retval;
}

/* http://linux.die.net/man/2/clock_gettime 
 * these top two require linking with realtime:
 * -lrt
 *
 * This returns milliseconds as a double since midnight, jan 1 1970 (unix epoch)
 *  keep the postgres port dir in mind for replacements:
 * http://doxygen.postgresql.org/dir_fd8b95245ffcce776715f180c056b450.html
 * */

#define SECONDS_TO_MILLISECONDS(time) ((double)time * 1000)
#define MICROSECONDS_TO_MILLISECONDS(time) ((double)time * 0.001)
#define NANOSECONDS_TO_MILLISECONDS(time) ((double)time * 0.000001)
double oly_timestamp( void ) 
{
#if _POSIX_TIMERS > 0
    struct timespec olytime;
#else
    struct timeval olytime;
#endif

#if _POSIX_TIMERS > 0
    if (clock_gettime(CLOCK_REALTIME_COARSE, &olytime) != 0)
#else
    if (gettimeofday(&olytime, NULL) != 0) 
#endif
    {
        perror(strerror(errno));
        exit(EXIT_FAILURE);
    }
#if _POSIX_TIMERS > 0
    return (SECONDS_TO_MILLISECONDS(olytime.tv_sec) + 
            NANOSECONDS_TO_MILLISECONDS(olytime.tv_nsec));
#else
    return (SECONDS_TO_MILLISECONDS(olytime.tv_sec) + 
            MICROSECONDS_TO_MILLISECONDS(olytime.tv_usec));
#endif
/* another good block for windows, but later, later. 
 * from here
 http://stackoverflow.com/questions/2414359/microsecond-resolution-timestamps-on-windows
 */
   /*
#ifndef __CYGWIN32__
double RealElapsedTime(void) { // returns 0 seconds first time called
   static struct timeval t0;
   struct timeval tv;
   gettimeofday(&tv, 0);
   if (!t0.tv_sec)
      t0 = tv;
   return tv.tv_sec - t0.tv_sec + (tv.tv_usec - t0.tv_usec) / 1000000.;
}
#else
#include <windows.h>
double RealElapsedTime(void) { // granularity about 50 microsecs on my machine
   static LARGE_INTEGER freq, start;
   LARGE_INTEGER count;
   if (!QueryPerformanceCounter(&count))
      FatalError("QueryPerformanceCounter");
   if (!freq.QuadPart) { // one time initialization
      if (!QueryPerformanceFrequency(&freq))
         FatalError("QueryPerformanceFrequency");
      start = count;
   }
   return (double)(count.QuadPart - start.QuadPart) / freq.QuadPart;
}
#endif
*/
}

