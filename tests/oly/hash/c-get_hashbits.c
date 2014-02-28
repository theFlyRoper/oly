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

typedef union oly_hash_union {
  size_t        sval[SIZE_HASH];
  uint32_t      ival[UINT32_HASH];
  unsigned char cval[CHAR_HASH];
} oly_hash;

int
main (int argc, char **argv){
  char                *hash_me = argv[1];
  charhash            corned_beef;
  data_length         hash_length = 0;
  oly_status          ostatus = OLY_OKAY;
  
  plan(1);
  if (argc != 2) {
    printf("Please provide exactly one argument.\n");
    return EXIT_FAILURE;
  }
  ostatus = get_str_hashlen((const unsigned char *)hash_me, &hash_length);
    
  printf("hash_me: %s, hash_length: %u\n", hash_me, hash_length);
  ostatus = get_hashbits((const bit_sequence *)hash_me, hash_length,
    &corned_beef);

  print_charhash(corned_beef);
  printf("SIZE_T: %u UCHAR: %u\n",SIZE_HASH, CHAR_HASH);
 
  plan(1);
  is_int(8, 8, "string=");
  if (ostatus == OLY_OKAY) {
    return EXIT_SUCCESS;
  } else {
    return EXIT_FAILURE;
  }
  
}

