/* oly_gettime.c - get currrent timestamp to the finest grain possible. License GPL2+ {{{
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
 *  keep the postgres port dir in mind for replacements:
 *http://doxygen.postgresql.org/dir_fd8b95245ffcce776715f180c056b450.html
 * */

#define ONE_BILLION_NANOSECONDS (double)1000000000.0
#define ONE_MILLION_MICROSECONDS (double)1000000.0
double oly_timestamp( OlyState *state ) 
{
#if _POSIX_TIMERS > 0
    struct timespec olytime;
#else
    struct timeval olytime;
#endif

#ifdef _POSIX_MONOTONIC_CLOCK
    if (clock_gettime(CLOCK_MONOTONIC_COARSE, olytime) != 0)
#elif _POSIX_TIMERS > 0
    if (clock_gettime(CLOCK_REALTIME, olytime) != 0)
#else
    if (gettimeofday(olytime, NULL) != 0) 
#endif
    {
        printf(strerror(errno));
        exit(EXIT_FAILURE);
    }
#if _POSIX_TIMERS > 0
    return (((double)olytime.tv_sec) + 
            (((double)olytime.tv_nsec)/ONE_BILLION_NANOSECONDS));
#else
    return ((double)olytime.tv_sec + 
            (((double)olytime.tv_usec)/ONE_MILLION_MICROSECONDS));
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
