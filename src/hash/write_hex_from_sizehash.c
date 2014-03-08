/* write_hex_from_sizehash.c - Write hex ascii from a sizehash. License GPL2+ {{{
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
write_hex_from_sizehash (FILE *f, const sizehash c, OlyState state){
  int       i=0;
  for (i = 0; (i < SIZE_HASH ); i++) {
#if SIZEOF_SIZE_T == 8
      if (fprintf(f, "%016zx", c[i]) <= 0) 
#else 
      if (fprintf(f, "%08zx", c[i]) <= 0) 
#endif
      {
        return OLY_ERR_FILEIO;
      };
  }
  return OLY_OKAY;
};

OlyStatus
print_hex_from_sizehash (const sizehash c, OlyState state)
{
  return write_hex_from_sizehash (stdout, c, state);
}


