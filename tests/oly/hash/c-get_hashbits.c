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

int
main (void){
  unsigned char       curr_bits = 8;
  size_t              curr_mask = 255;
  char                input[8] = 0;
  int                 result = 0;
  char                *hash_me = "jonathan";
  oly_hash            corned_beef;
  data_length         hash_length = 0;
  oly_status          ostatus = OLY_OKAY;

  char                checkme[] = "@@@@";
  
  plan(9);
  hash_length = ((data_length)strlen(hash_me)*CHAR_BIT);
  
  ostatus = get_hashbits((const bit_sequence *)hash_me, hash_length,
    (bit_sequence *)corned_beef.charval);

  print_result((const char *)corned_beef.charval);
  printf("SIZE_T: %u UCHAR: %u\n",SIZE_HASH, CHAR_HASH);
  printf("before treatment: %04x\n",corned_beef.intval[0]);
  printf("after treatment: %08x\n",char_to_size(&corned_beef.charval));
  printf("Macro version: %08x\n", );
 
  plan(1);
  is_int(8, 8, "string=");
  return EXIT_SUCCESS;
  
}

