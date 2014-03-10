/* oly_timestamp.c - get currrent timestamp to the finest grain possible. License GPL2+ {{{
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

/* http://linux.die.net/man/2/clock_gettime 
 * these top two require linking with realtime:
 * -lrt
 *
 * This returns milliseconds as a double since midnight, jan 1 1970 (unix epoch)
 *  keep the postgres port dir in mind for replacements:
 * http://doxygen.postgresql.org/dir_fd8b95245ffcce776715f180c056b450.html
 * */
#include "oly/common.h"
#include "oly/state.h"
#include "oly/core.h"

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
