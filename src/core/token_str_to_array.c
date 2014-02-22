/* token_str_to_array.c - Transforms a tokenized string into an array License GPL2+ {{{
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
#include "oly/wrappers.h"

/* allocates a char ** array and copies each token into it. */
char **token_str_to_array(char *s, char *delims, Oly_Status *status) 
{
  const size_t    arr_size = count_tokens(s,delims), 
                  num_chars = count_nondelim_chars(s,delims);
  size_t          arr_ptr = 0, strsize = 0;
  /* we need arr_size char pointers and arr_size (for the EOLs) + num_chars chars. */
  char          **result = (char **)xmalloc(
                  ((arr_size * sizeof(char *)) 
                   + ((arr_size + num_chars) * sizeof(char)))),
                 *next, *result_position;

  *status = OLY_OKAY;
  result_position = (char *)(result + (arr_size * sizeof(char *)));
  if ( delims != NULL ) {
    while ( s != NULL && *(s += strspn(s, delims)) != '\0' ) {
      next = strpbrk(s, delims);
      if (next != NULL) {
        strsize = (next - s);
      } else {
        strsize = strlen(s);
      };
      strncpy(result_position, s, strsize);
      *(result_position + strsize) = '\0';
      result[arr_ptr++] = result_position;
      result_position += (strsize +1);
      s = next;
    }
  }
  return result;
}

