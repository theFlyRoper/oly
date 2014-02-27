/* char_to_size - convert 8 chars into a single uint32_t and return it {{{
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
#include <limits.h>

#include "oly/core.h"
#include "oly/hash.h"

/* because this counts through an array, this should work on any platform. 
 * we will find out later. For now I assume it will.  */

/* added note: addressing misbehaves over 32, so for now, we just deal with 32. */
/* for now, on hiatus.
static uint64_t char_to_uint64(const unsigned char *c){
  union {
    uint32_t i32[2];
    uint64_t i64;
  } xform;
  xform.i32[0] = char_to_uint32(c+4);
  xform.i32[1] = char_to_uint32(c);
  printf("xform: %016llx\n", xform.i64);
  return xform.i64;
}
*/

oly_status char_to_size(const unsigned char *c, size_t *res){
  size_t        i = 0, j = (sizeof(size_t)-1);
  oly_status    status = OLY_OKAY;
  *res = 0;
  for (i = 0; (i <= j); i++) {
    *res |= ((size_t)c[i] << (CHAR_BIT * (j - i)));
  }
  return status;
}


