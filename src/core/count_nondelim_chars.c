/* count_nondelim_chars.c - count tokens in a c string License GPL2+ {{{
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

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "oly/core.h"

size_t
count_nondelim_chars (char *s, char *delims)
{
  size_t  current_count = 0;
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

