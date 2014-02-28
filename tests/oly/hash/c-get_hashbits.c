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
main( int argc, char **argv ){
  unsigned char       curr_bits = 8;
  size_t              curr_mask = 255;
  int                 i_like_turtles = 0;
  char                *hash_me = argv[1];
  charhash            corned_beef;
  data_length         hash_length = 0;
  oly_status          ostatus = OLY_OKAY;

  char                checkme[] = "@@@@";
  
  if (argc != 2) {
    printf("Please provide exactly 1 argument.  Exiting...\n");
    exit(EXIT_FAILURE);
  }
  hash_length = ((data_length)strlen(hash_me)*CHAR_BIT);
  
  ostatus = get_hashbits((const bit_sequence *)hash_me, hash_length,
    (bit_sequence *)corned_beef);

  print_charhash(corned_beef);
  return EXIT_SUCCESS;
  
}

