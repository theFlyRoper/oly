/* get_str_hashlen test License GPL2+ {{{
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
    const char *test1 = "ABCDEFG", test3[] = {
                    0x30, 0x60, 0x4A, 0x6F, 0x6E, 0x06, 0x57, 0x41,
                    0x73, 0x68, 0x45, 0x52, 0x65, 0x21, 0x26, 0x26, '\0'},
                  *fileloc = "/data/user_account_testdata.csv";
    char filename[BUFSIZ],*test2;
    FILE         *test2file;
    size_t        test_data_dir_len = strlen(TESTDATADIR), result;
    data_length   hash_length ;
    OlyStatus    ostatus = OLY_OKAY;

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
  
}

