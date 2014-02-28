/* char_to_uint32 - transform all chars from a hash return to uints {{{
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
#include "oly/state.h"
static uint32_t 
char_to_uint32(const charhash c)
{
  uint32_t        i = 0, j = (sizeof(uint32_t)-1);
  uint32_t        res = 0;
  for (i = 0; (i <= j); i += sizeof(char)) {
    res |= ((uint32_t)c[i] << (CHAR_BIT * (j - i)));
  }
  return res;
};

oly_status 
hash_char_to_hash_uint32(const unsigned char *c, uint32_t *result[])
{
  uint32_t        i = 0;
  oly_status      status = OLY_OKAY;
  for (i = 0; (i < CHAR_HASH); i += sizeof(uint32_t)) {
    *result[i] = char_to_uint32((const unsigned char *)&c[i]);
  }
  return status;
}

