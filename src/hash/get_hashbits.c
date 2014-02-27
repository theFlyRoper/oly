/* oly_hash - return the sha3 hash of the bitstream input {{{
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

#include <lib/sha_three/KeccakNISTInterface.h>
#include "oly/common.h"

#include "oly/hash.h"

oly_status 
get_hashbits( const bit_sequence *data, data_length len, bit_sequence *hash ) 
{
  if (Hash(OLY_HASH_BITS, data, len, hash) != SUCCESS) {
    return OLY_ERR_UNSPECIFIED;
  };
  return OLY_OKAY;
}

