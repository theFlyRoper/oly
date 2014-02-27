/* get_hashbits test License GPL2+ {{{
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
  unsigned char       curr_bits = 8;
  size_t              curr_mask = 255;
  int                 i_like_turtles = 0;
  char                *hash_me = "jonathan";
  hash_val            corned_beef;
  data_length         hash_length = 0;
  oly_status          ostatus = OLY_OKAY;

  char                checkme[] = "@@@@";
  
  plan(9);
  hash_length = ((data_length)strlen(hash_me)*CHAR_BIT);
  
  ostatus = oly_hash((const bit_sequence *)hash_me, hash_length,
    (bit_sequence *)corned_beef.char_hash);

  print_result((const char *)corned_beef.char_hash);
  printf("ULONG: %u UINT: %u UCHAR: %u\n",ULONG_HASH_ARRAY, 
      UINT_HASH_ARRAY, CHAR_HASH_ARRAY);
  
  printf("before treatment: %04x\n",corned_beef.uint_hash[0]);

  printf("after treatment: %04x\n",n_to_uint(&corned_beef.char_hash));
 
  plan(1);
  is_int(8, 8, "string=");
  return EXIT_SUCCESS;
  
}

