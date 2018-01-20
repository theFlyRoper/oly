/* oly_hash.h -- Oly inteface to hash functions and tables, defaults to SHA3 (Keccak) {{{
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.
   }}} */

#ifndef OLY_OLY_HASH_H
#define OLY_OLY_HASH_H 1

#include "oly/common.h"

#include <string.h>
#include <limits.h>
#include <math.h>

#include <lib/sha_three/KeccakNISTInterface.h>
#include "oly/olytypes.h"
#include "oly/state.h"

/* the NIST interface for SHA 3 defines a few simple data types, 
 * which Oly will use for any other hash function as well.
 * They are:
 *    bit_sequence (from NIST BitSequence)
 *    data_length (from NIST DataLength)
 *    hash_return (from NIST HashReturn)
 *    hash_state
 * Oly uses 224 bit hashing.  It uses an array of 32 chars, using one char per hash table expansion, minus one bit per char.
 *
 */

#define OLY_HASH_BITS 224
#define CHAR_HASH ((OLY_HASH_BITS/CHAR_BIT) + (32/CHAR_BIT))
#define SIZE_HASH (OLY_HASH_BITS/(SIZEOF_SIZE_T*CHAR_BIT))
#define OLY_MASK 0x128

typedef unsigned char hash[CHAR_HASH];

typedef HashReturn hash_return;
typedef BitSequence bit_sequence;
typedef DataLength data_length;
struct oly_hash_table_struct;
typedef struct oly_hash_table_struct OlyHashTable;

OlyStatus get_hashbits( const bit_sequence *data, data_length len, bit_sequence *hash ) ;
OlyStatus get_str_hashlen(const unsigned char *c, data_length *result);

/* print_<x> is a wrapper around write_<x> and can be found
 * in the write_<x> files. */
OlyStatus print_hex_from_hash (const hash c, OlyState *state);
OlyStatus write_hex_from_hash (FILE *f, const hash c, OlyState *state);
OlyStatus read_hash_from_hex  (const char *s, hash c, OlyState *state);

#endif /* OLY_OLY_HASH_H */

