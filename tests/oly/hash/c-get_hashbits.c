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
#include "oly/globals.h"
#include "tests/tap/basic.h"
#include "pvt_core.h"

/* HashReturn Init(hashState *state, int hashbitlen);
HashReturn Update(hashState *state, const BitSequence *data, 
    DataLength databitlen);
HashReturn Final(hashState *state, BitSequence *hashval);
HashReturn Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval); */

int
main( int argc, char **argv ){
    char                *hash_me = argv[1];
    hash            corned_beef;
    data_length         hash_length = 0;
  
    if (argc != 2) 
    {
        printf("Please provide exactly 1 argument.  Exiting...\n");
        exit(EXIT_FAILURE);
    }
    if (OLY_OKAY != get_str_hashlen((const unsigned char *)hash_me, 
            &hash_length)) 
    {
        exit(EXIT_FAILURE);
    }
    
    if (OLY_OKAY != get_hashbits((const bit_sequence *)hash_me,
            hash_length, (bit_sequence *)corned_beef)) 
    {
        exit(EXIT_FAILURE);
    }

    if ( OLY_OKAY == print_hex_from_hash(corned_beef, NULL)) 
    {
        exit(EXIT_SUCCESS);
    }
    else 
    {
        exit(EXIT_FAILURE);
    }
}

