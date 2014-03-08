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

#include <string.h>
#include <limits.h>

#include <lib/sha_three/KeccakNISTInterface.h>
#include "oly/state.h"

/* the NIST interface for SHA 3 defines a few simple data types, 
 * which Oly will use for any other hash function as well.
 * They are:
 *    bit_sequence (from NIST BitSequence)
 *    data_length (from NIST DataLength)
 *    hash_return (from NIST HashReturn)
 *    hash_state
 * Oly uses 256 bit hashing.  It transforms the output from 64 chars to an array of
 * 256/(sizeof(size_t)*CHAR_BIT).  I think this is a flexible solution.
 *
 */
#define OLY_HASH_BITS 256 
#define SIZE_HASH (OLY_HASH_BITS/(SIZEOF_SIZE_T*CHAR_BIT))
#define CHAR_HASH (OLY_HASH_BITS/CHAR_BIT)

/* little endian = low byte leftmost. big endian = low byte rightmost. */

typedef size_t        sizehash[SIZE_HASH];
typedef unsigned char charhash[CHAR_HASH];

typedef HashReturn hash_return;
typedef BitSequence bit_sequence;
typedef DataLength data_length;

OlyStatus get_hashbits( const bit_sequence *data, data_length len, bit_sequence *hash ) ;
OlyStatus get_str_hashlen(const unsigned char *c, data_length *result);

/* print_<x> is a wrapper around write_<x> and can be found
 * in the write_<x> files. */
OlyStatus print_hex_from_sizehash (const sizehash c, oly_state state);
OlyStatus print_hex_from_charhash (const charhash c, oly_state state);

OlyStatus write_hex_from_sizehash (FILE *f, const sizehash c, oly_state state);
OlyStatus write_hex_from_charhash (FILE *f, const charhash c, oly_state state);
OlyStatus read_sizehash_from_hex  (const char *s, sizehash c, oly_state state);
OlyStatus read_charhash_from_hex  (const char *s, charhash c, oly_state state);

OlyStatus char_to_size(const unsigned char *c, size_t *res);
OlyStatus hash_char_to_hash_size(const unsigned char *c, size_t result[]);

#endif /* OLY_OLY_HASH_H */

