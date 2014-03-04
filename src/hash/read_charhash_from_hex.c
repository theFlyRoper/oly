/* read_charhash_from_hex.c - Read hex ascii and make a charhash. License GPL2+ {{{
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

#include <ctype.h>
#include "oly/common.h"

#include "oly/core.h"
#include "oly/hash.h"

oly_status 
read_charhash_from_hex  (const char *s, charhash c, oly_state state)
{
  int   i = 0;
  if (strlen(s) < ((OLY_HASH_BITS/(CHAR_BIT/2))))
  {
      return OLY_ERR_READHEX;      
  };
  for (i = 0; (i<(OLY_HASH_BITS/CHAR_BIT)); 
      s+=2, i++) {
    if (sscanf(s, "%02hhx", &c[i]) != 1) {
      c = memset(c, 0x00, (size_t)(OLY_HASH_BITS/CHAR_BIT));
      return OLY_ERR_READHEX;      
    }
  }
  return OLY_OKAY;
}

