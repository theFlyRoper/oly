/* waitpid.c -- implement waitpid() for architectures without it
  
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
*/
#if HAVE_CONFIG_H
#  include <olyconf.h>
#endif

#include <sys/types.h>		/* for pid_t */
#if HAVE_SYS_WAIT_H
#  include <sys/wait.h>
#endif

pid_t
waitpid (pid_t pid, int *pstatus, int options)
{
  pid_t result;

  do {
    result = wait (pstatus);
  } while  (result >= 0 && result != pid);

  return result;
}
