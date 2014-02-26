/* count_tokens.c - count tokens separated by any char in a delimiter series License GPL2+ {{{
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
#include "oly/core.h"

size_t
count_tokens (char *s, char *delims)
{
  size_t  current_count = 0;

  /* loop over s, adding 1 to current_count for each iteration. */
  if ( delims != NULL ) {
    while ( s != NULL && *(s += strspn(s, delims)) != '\0' ) {
      current_count++;
      s = strpbrk(s, delims);
    }
  }
  return current_count;
}

