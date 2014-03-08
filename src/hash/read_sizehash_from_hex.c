/* read_sizehash_from_hex.c - Read hex ascii and make a sizehash. License GPL2+ {{{
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

#include "oly/oly.h"
#include "oly/hash.h"

OlyStatus 
read_sizehash_from_hex  (const char *s, sizehash c, OlyState state)
{
  int   i = 0;
  if (strlen(s) < ((OLY_HASH_BITS/(CHAR_BIT/2))))
  {
      return OLY_ERR_READHEX;      
  };
  for (i = 0; (i<(SIZE_HASH)); 
      s+=(SIZE_HASH*2), i++) {
#if SIZEOF_SIZE_T == 8
      if (sscanf(s, "%016zx", &c[i]) != 1) {
#else 
      if (sscanf(s, "%08zx", &c[i]) != 1) {
#endif
      c = memset(c, 0x00, (size_t)(OLY_HASH_BITS/CHAR_BIT));
      return OLY_ERR_READHEX;      
    }
  }
  return OLY_OKAY;
}

