/* hash_char_to_hash_uint32 test License GPL2+ {{{
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

int
main (void){
    charhash            input;
    char                *hash_me = "jonathan";
    int32hash            result;
    data_length         hash_length ;
    oly_status          ostatus = OLY_OKAY;
    
    ostatus = get_str_hashlen(hash_me, &hash_length);

    assert((test_data_dir_len + strlen((char *)fileloc) + 1 ) < BUFSIZ);
    
    strcpy(filename, TESTDATADIR);
    strcpy(&filename[test_data_dir_len-1], fileloc);
    test2file = fopen(filename, "rb");
    if( test2file == 0 ) {
        fprintf(stderr, "Could not open file \"%s\"\n", filename);
        exit(EXIT_FAILURE);
    }
    fseek(test2file, 0, SEEK_END);
    test_data_dir_len = ftell(test2file);
    fseek(test2file, 0, SEEK_SET);
    
    test2 = (char *)xmalloc (test_data_dir_len + 1);
    test2[test_data_dir_len] = '\0';
    result = (long)fread(test2, 1, test_data_dir_len, test2file);
    if (result != test_data_dir_len)  {
        fprintf(stderr, "Error reading file \"%s\"\n", filename);
        exit (EXIT_FAILURE);
    }
    plan(3);
    ostatus = get_str_hashlen((const unsigned char *)test1, &hash_length);
    if (ostatus != OLY_OKAY) {
        fprintf(stderr, "Test 1 failed.\n");
        exit (EXIT_FAILURE);
    }
    is_int(56, hash_length, "Test1, should be 56. hash_length: %u",   (int)hash_length);
    ostatus = get_str_hashlen((const unsigned char *)test2, &hash_length);
    if (ostatus != OLY_OKAY) {
        fprintf(stderr, "Test 2 failed.\n");
        exit (EXIT_FAILURE);
    }
    diag("Test 2 opens this file:\n%s", filename);
    is_int(74352, hash_length, "Test2, should be 74352. hash_length: %u",
        (int)hash_length);
    ostatus = get_str_hashlen((const unsigned char *)test3, &hash_length);
    is_int(128, hash_length, "Test3, should be 128. hash_length: %u", (int)hash_length);
    return EXIT_SUCCESS;
  ostatus = get_hashbits((const bit_sequence *)hash_me, hash_length,
    (bit_sequence *)input);
  print_hex_from_charhash(input);
  ostatus = hash_char_to_hash_uint32((const unsigned char *)input,&result);
  print_int32hash(result);
  return EXIT_SUCCESS;
  
}

