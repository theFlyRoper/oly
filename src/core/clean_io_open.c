/* clean_io_open.c - precleans the std* global streams. License GPL2+ {{{
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

#include <sys/resource.h>
#include <sys/stat.h>
#include <errno.h>
#include <paths.h>
#include <limits.h>

#include "oly/core.h"
#include "pvt_core.h"

/* open_devnull and clean_io_open
 * are adapted from the Secure Programming 
 * Cookbook, John Viega and Matt Messier.
 * 2003, O'Reilly Press, 
 * ISBN: 0-596-00394-3
 */

static int open_devnull(int fd);

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

