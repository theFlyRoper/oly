/* char_to_size test License GPL2+ {{{
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

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "oly/state.h"
#include "oly/hash.h"
#include "tests/tap/basic.h"

/* HashReturn Init(hashState *state, int hashbitlen);
HashReturn Update(hashState *state, const BitSequence *data, 
    DataLength databitlen);
HashReturn Final(hashState *state, BitSequence *hashval);
HashReturn Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval); */

typedef union hash_t {
  unsigned long long ulong_hash[ULONG_HASH_ARRAY];
  unsigned int uint_hash[UINT_HASH_ARRAY];
  unsigned char char_hash[CHAR_HASH_ARRAY];
} hash_val;

int
main (void){
  hash_val    test1 = {0xEFBEADBA}, test2 = {0xEDADBE00},
              test3 = {0xED0BEDFE};
  unsigned int result1 = 0xBAADBEEF, result2 = 0x00BEADED, 
               result3 = 0xFEED0BED,
                output_val = 0;
  diag("TODO: rearrange this to add a bigendian chunk.");
  plan(3);
  output_val = char_to_size(test1.char_hash);
  is_hex(result1, output_val, "%08X: Probably mad cow disease.", output_val);
  output_val = char_to_size(test2.char_hash);
  is_hex(result2, output_val, "%08X: Ah, lovely work.", output_val);
  output_val = char_to_size(test3.char_hash);
  is_hex(result3, output_val, "%08X: What, you mean like fertilizer?", output_val);
  return EXIT_SUCCESS;
  
}

