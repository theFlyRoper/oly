/* token_str_to_array test License GPL2+ {{{
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

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "oly/common.h"
#include "oly/state.h"
#include "oly/oly.h"

static char **
token_str_to_array(char *s, char *delims, unsigned int *count_chars, 
        unsigned int *count_tokens, OlyStatus *status) ;
#include "src/core/token_str_to_array.c"

/* MAIN */
int
main( int argc, char **argv ){
    const int       test1_size = 4, test2_size=1, test3_size=1, test4_size=1, test5_size = 3, test6_size = 1, test7_size = 3, test8_size = 2;
    size_t          i = 0, characters = 0, tokens = 0, string_len = 0;
    char            *program_name = argv[0];
    char            delim1234[] = ":", delim5678[] = ":|";
    char            test1[] = ":::::ten:::chars:::!!::3120::::::",
                    test2[] = "there can be only one.",
                    test3[] = ":one, but with boundaries.:",
                    test4[] = "one, no start.:",
                    test5[] = "|da:big|three|",
                    test6[] = ":one, no end.",
                    test7[] = "seven:seven|seven",
                    test8[] = "ate:nine",
                    **result,
                    *output1[] = { "ten", "chars", "!!", "3120" },
                    *output2[] = { "there can be only one." },
                    *output3[] = { "one, but with boundaries." },
                    *output4[] = { "one, no start." },
                    *output5[] = { "da", "big", "three" },
                    *output6[] = { "one, no end." },
                    *output7[] = { "seven", "seven", "seven" },
                    *output8[] = { "ate", "nine" };
    OlyStatus      status;

    
    plan(32);
    diag("round one. four is_string results, gets the right count for characters and tokens.");
    result = token_str_to_array(test1, delim1234, &characters, &tokens, &status);
    for ( i=0; i < test1_size; i++ ) {
        is_string(output1[i], result[i], "result[%i](%s) = output[%i](%s)", i, 
                result[i], i, output1[i] );
    }
    is_int(4, tokens, "should be 4 tokens.");
    is_int(14, characters, "Ten characters.");
    diag(" round two.  one big chunk, no delimiters. 24 chars, 1 token.");
    result = token_str_to_array(test2, delim1234, &characters, &tokens, &status);
    for ( i=0; i < test2_size; i++ ) {
        is_string(output2[i], result[i], "result[%i](%s) = output[%i](%s)", i, 
                result[i], i, output2[i] );
    }
    is_int(1, tokens, "one token.");
    string_len = strlen(result[0]);
    is_int( string_len , characters, "%i characters, by strlen.", string_len);
    
    diag("round three.  one big chunk. 25 chars, 1 token.");
    result = token_str_to_array(test3, delim1234, &characters, &tokens, &status);
    for ( i=0; i < test3_size; i++ ) {
        is_string(output3[i], result[i], "result[%i](%s) = output[%i](%s)", i, 
                result[i], i, output3[i] );
    }
    is_int(1, tokens, "one token.");
    is_int( strlen(result[0]) , characters, "%i characters, by strlen.", 
            strlen(result[0]) );
    
    /* */
    diag("round four.  one big chunk. 25 chars, 1 token.");
    result = token_str_to_array(test4, delim1234, &characters, &tokens, &status);
    for ( i=0; i < test4_size; i++ ) 
    {
        is_string(output4[i], result[i], "result[%i](%s) = output[%i](%s)", i, 
                result[i], i, output4[i] );
    }
    is_int(1, tokens, "one token.");
    is_int( strlen(result[0]) , characters, "%i characters, by strlen.", 
            strlen(result[0]) );
    
    diag("Five, three tokens n stuff.");
    result = token_str_to_array(test5, delim5678, &characters, &tokens, &status);
    for ( i=0; i < test5_size; i++ ) 
    {
        is_string(output5[i], result[i], "result[%i](%s) = output[%i](%s)", i, 
                result[i], i, output5[i] );
    }
    is_int(3, tokens, "three tokens.");
    is_int(10, characters, "Ten characters.");
    
    diag("Six, one token at the front.");
    result = token_str_to_array(test6, delim5678, &characters, &tokens, &status);
    for ( i=0; i < test6_size; i++ ) {
        is_string( output6[i], result[i], "result[%i](%s) = output[%i](%s)", i, 
                result[i], i, output6[i] );
    }
    is_int(1, tokens, "ONEEEE IS THE LONELEYESTTT");
    is_int(12, characters, "numba");
    
    diag("Seven, no tokens on ends.");
    result = token_str_to_array(test7, delim5678, &characters, &tokens, &status);
    for ( i=0; i < test7_size; i++ ) {
        is_string( output7[i], result[i], "result[%i](%s) = output[%i](%s)", i, 
                result[i], i, output7[i] );
    }
    is_int(3, tokens, "three tokens.");
    is_int(15, characters, "fifteen characters.");
    
    diag("Eight, no tokens on ends, two delimiters.");
    result = token_str_to_array(test8, delim5678, &characters, &tokens, &status);
    for ( i=0; i < test8_size; i++ ) {
        is_string( output8[i], result[i], "result[%i](%s) = output[%i](%s)", i, 
                result[i], i, output8[i] );
    }
    is_int(2, tokens, "two tokens.");
    is_int(7, characters, "seven characters.");
    return EXIT_SUCCESS;
}

