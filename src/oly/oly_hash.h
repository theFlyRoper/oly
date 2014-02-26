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

#ifndef BUFSIZ
#define BUFSIZ 8192
#endif /* BUFSIZ */

#include <sha_three/KeccakNISTInterface.h>
#include "oly/state.h"

/* the NIST interface for SHA 3 defines a few simple data types, 
 * which Oly will use for any other hash function as well.
 * They are:
 *    bit_sequence (from BitSequence)
 *    data_length (from DataLength)
 *    hash_return (from HashReturn)
 *    hash_state
 * Oly uses 256 bit hashing.  I see no reason to change this.  Max addressing
 * on most systems is 64 bits so odds are Oly will only ever use <32 or <64 bits, 
 * that is, unsigned char[4] or unsigned char[8] for addresssing 
 * (assuming char = 8 bits).
 */

#define OLY_HASH_SIZE 256 
#define ULONG_HASH_ARRAY (OLY_HASH_SIZE/(sizeof(unsigned long long)*CHAR_BIT))
#define UINT_HASH_ARRAY (OLY_HASH_SIZE/(sizeof(unsigned int)*CHAR_BIT))
#define CHAR_HASH_ARRAY (OLY_HASH_SIZE/CHAR_BIT)

typedef HashReturn hash_return;
typedef BitSequence bit_sequence;
typedef DataLength data_length;

oly_status nchar_bitnot(const char *s,  char *not_s, size_t len);
oly_status nchar_bitor (const char *s1, const char *s2, char *out, size_t len);
oly_status nchar_bitand(const char *s1, const char *s2, char *out, size_t len);
size_t memory_left_now(void);
size_t getMemorySize( );
void print_result(const char *c);

oly_status str4_to_int(const char *c, int *res);
oly_status oly_hash( const bit_sequence *data, data_length len, bit_sequence *hash ) ;

#endif // OLY_OLY_HASH_H

