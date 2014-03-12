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

int
main( int argc, char **argv ){
    charhash            input;
    charhash            read;
    const unsigned char *hash_me = (const unsigned char *)argv[1];
    const unsigned char *read_me = (const unsigned char *)argv[2];
    data_length         hash_length ;
    int                 strcmp_val;
    char            *locale = (char *)"root",  *charset = NULL;
    oly = init_oly(argv[0], TEST_PKGDATADIR, charset, locale);
  if (argc != 3) 
  {
    printf("2 args\n");
    exit(EXIT_FAILURE);
  }
  if (OLY_OKAY != read_charhash_from_hex (read_me, read, oly))
  {
    printf("Problem with reading the charhash.\n%s",read);
    exit(EXIT_FAILURE);
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
  strcmp_val = memcmp(input,read,(size_t)(OLY_HASH_BITS/CHAR_BIT));
  if (strcmp_val!=0) {
    printf("Input and read are different.  Value read:\n");
    print_hex_from_charhash(read, oly);
    printf("\nValue hashed:\n");
    print_hex_from_charhash(input, oly);
    printf("\nStrcmp_val: %i\n", strcmp_val);
    exit(EXIT_FAILURE);
  }
  printf("Success!\n");
  return EXIT_SUCCESS;
  
}

