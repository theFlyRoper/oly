/* oly_get_ochar_args.c - flip a **char argv into a **ochar argv {{{
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

#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif

#include "oly/common.h"

#include <unicode/ustdio.h>
#include <unicode/ustring.h>

#include <string.h>
#include <stdio.h>

#include "oly/core.h"
#include "oly/state.h"

oly_status
oly_getocharArgs(ochar ***result, char **source, int32_t argc){
  size_t          i = 0, storage = 0, arr_size =1, arr_ptr = 0;
  char            *var = NULL;
  ochar           **curr, *ptr;

  for (i = 0;  ((var = source[i]) != 0);  i++) {
    storage += ((strlen(var)+1) * sizeof(ochar *));
    arr_size++;
  }
    storage += (arr_size * sizeof(ochar *));

  if (!(curr = (ochar **)xmalloc(storage))) {
    abort(  );
  }
  curr[arr_size - 1] = 0;
  
  ptr = (ochar *)curr + (arr_size * sizeof(ochar *));
  for (i = 0;  (var = source[i]) != 0;  i++) {
    curr[arr_ptr++] = ptr;
    storage = ((strlen(var)+1) * sizeof(ochar));
    u_uastrncpy( ptr, var, storage );
    ptr += (storage * sizeof(ochar *));
  }

  /* C passes in a copy of the pointer in the original value. Thus, 
   * when you set the pointed-to value to curr, you get the right behavior. 
   */
  (*result) = curr;
  return OLY_OKAY;
}
