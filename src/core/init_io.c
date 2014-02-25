/* init_io.c -- start the unicode IO streams from ICU {{{
  
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

#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif

#include <unicode/ustdio.h>
#include <unicode/ubrk.h>
#include <stdio.h>

#include "oly/common.h"
#include "oly/output.h"
#include "oly/state.h"
#include "oly/list.h"
#include "oly/syntax.h"
#include "oly/oly.h"
#include "oly/core.h"

void
init_io(const char *locale, const char *codepage) {
  char    err_buffer[BUFSIZ];
  int     errnum=0;

  if ( u_stderr == NULL ) {
      u_stderr=u_finit(stderr, locale,  NULL /*codepage */);
      if(!u_stderr) {
          if ( strerror_r( errnum, err_buffer, BUFSIZ ) == 0 ) {
              printf("%s", err_buffer);
          }
          exit(1);
      }
  }
  if( u_stdout == NULL ) {
      u_stdout = u_finit(stdout, locale,  NULL /*codepage */);
      if(!u_stdout) {
          if ( strerror_r( errnum, err_buffer, BUFSIZ ) == 0 ) {
              printf("%s", err_buffer);
          }
          exit(1);
      }
  }
  if(u_stdin == NULL) {
      u_stdin = u_finit(stdin, locale, NULL);
      if(!u_stdin) {
          if ( strerror_r( errnum, err_buffer, BUFSIZ ) == 0 ) {
              printf("%s", err_buffer);
          }
          exit(1);
      }
  }
}

