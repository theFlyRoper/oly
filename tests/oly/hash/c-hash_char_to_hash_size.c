/* hash_char_to_hash_size test License GPL2+ {{{
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

typedef union oly_hash_union {
  size_t        sval[SIZE_HASH];
  unsigned char cval[CHAR_HASH];
} oly_hash;

int
main( int argc, char **argv ){
    charhash              input;
    const unsigned char  *hash_me = (const unsigned char *)argv[1];
    sizehash              result;
    char            *locale = (char *)"root",  *charset = NULL;
    data_length           hash_length ;
    init_oly(argv[0], TEST_PKGDATADIR, charset, locale, &oly);
  
    if (argc != 2) {
        printf("Takes 1 argument, to be hashed. exiting...");
    }
    
    if (OLY_OKAY != get_str_hashlen(hash_me, &hash_length)) 
    {
        exit(EXIT_FAILURE);
    }

    if (OLY_OKAY != get_hashbits((const bit_sequence *)hash_me,
            hash_length, (bit_sequence *)&input)) 
    {
        exit(EXIT_FAILURE);
    }
    if (OLY_OKAY != print_hex_from_charhash((const charhash *)&input, oly)) {
        exit(EXIT_FAILURE);
    }
    printf("\n");
    if (OLY_OKAY != hash_char_to_hash_size(
            (const unsigned char *)input, (size_t **)&result)) 
    {
        exit(EXIT_FAILURE);
    }
    if (OLY_OKAY != print_hex_from_sizehash((const sizehash *)result, oly)) 
    {
        fprintf(stderr, "Test 2 failed.\n");
        exit (EXIT_FAILURE);
    }
    printf("\n");
    return EXIT_SUCCESS;
    
}

