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

oly_status char_to_size(const unsigned char *c, size_t *res){
  size_t        i = 0, j = (sizeof(size_t)-1);
  oly_status    status = OLY_OKAY;
  *res = 0;
  for (i = 0; (i <= j); i++) {
    *res |= ((size_t)c[i] << (CHAR_BIT * (j - i)));
  }
  return status;
}
  
oly_status hash_char_to_hash_size(const unsigned char *c, size_t result[]){
  size_t        i = 0, j = 0;
  oly_status    status = OLY_OKAY;
  
  for (i = 0, j = 0; (j < SIZE_HASH); i += sizeof(size_t), j++) {
    status = char_to_size(&c[i], &result[j]);
  }
  return status;
}


