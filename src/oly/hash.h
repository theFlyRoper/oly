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

#ifndef BUFSIZ
#define BUFSIZ 8192
#endif /* BUFSIZ */

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

typedef union oly_hash_type {
  size_t        intval[SIZE_HASH];
  unsigned char charval[CHAR_HASH];
} oly_hash;

#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
/* little endian = low byte leftmost. big endian = low byte rightmost. */
#define POSITION(i, j) result |= ((size_t)input[i] << j) ;
/* res |= ((unsigned int)c[i] << (CHAR_BIT * (j - i))); */
#if SIZEOF_SIZE_T == 8
#define CHAR_TO_SIZE(input, result) \
    POSITION(0, 0) POSITION(1, 8) POSITION(2, 16) POSITION(3, 24) \
    POSITION(4, 32) POSITION(5, 40) POSITION(6, 48) POSITION(7, 56) 
#else
#define CHAR_TO_SIZE(input, result) ( \
    POSITION(0, 0) POSITION(1, 8) POSITION(2, 16) POSITION(3, 24) )
#endif
#pragma message "CHARTOSIZE=" VALUE(CHAR_TO_SIZE(hi,lo))
/**/
typedef HashReturn hash_return;
typedef BitSequence bit_sequence;
typedef DataLength data_length;

oly_status nchar_bitnot(const char *s,  char *not_s, size_t len);
oly_status nchar_bitor (const char *s1, const char *s2, char *out, size_t len);
oly_status nchar_bitand(const char *s1, const char *s2, char *out, size_t len);
size_t memory_left_now(void);
size_t getMemorySize( );
void print_result(const char *c);


oly_status get_hashbits( const bit_sequence *data, data_length len, bit_sequence *hash ) ;

#endif /* OLY_OLY_HASH_H */

