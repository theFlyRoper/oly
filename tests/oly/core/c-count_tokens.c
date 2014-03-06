/* count tokens test License GPL2+ {{{
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
#include "oly/core.h"

static int32_t count_tokens (char *s, char *delims);
static int32_t count_nondelim_chars (char *s, char *delims);
static char **token_str_to_array(char *s, char *delims, int *count_chars, 
        int *count_tokens, oly_status *status) ;

#include "core/token_functions.c"
/* MAIN */
int
main( int argc, char **argv ){
  size_t         len       = 0;
  char            *program_name = argv[0];
  char            delim1234[] = ":", delim5678[] = ":|", delim9_15[] = ",|:",
                  delim16[] = "\0";
  char            test1[] = "two:tokens", test2[] = ":four:tokens:with:leader",
                  test3[] = "four:tokens:with:follower:",
                  test4[] = ":six:tokens:with:leader:and:follower:",
                  test5[] = "two:kinds|of:delimiter",
                  test6[] = "multiple|delimiters:|:between:|tokens",
                  test7[] = ":lead:followers:and:multiple|delimiters::between:|tokens:",
                  test8[] = "::::||||",
                  test9[] = "three|kinds:of,delimiter",
                  test10[] = ":,||three|kinds:of,delimiter,with|leader",
                  test11[] = "", test12[] = "one big token", 
                  /* no test string for 13 because it tests null passed as a value. */
                  test14[] = "Check it.",
                  test16[] = "EOL for delim.";
 
  plan(16);
  diag("delimiters for 1 2 3 and 4=\"%s\"", delim1234);
  is_int(2, count_tokens(test1, delim1234), "string=%s", test1);
  is_int(4, count_tokens(test2, delim1234), "string=%s", test2);
  is_int(4, count_tokens(test3, delim1234), "string=%s", test3);
  is_int(6, count_tokens(test4, delim1234), "string=%s", test4);
  diag("delimiters for 5 6 7 and 8=\"%s\"", delim5678);
  is_int(4, count_tokens(test5, delim5678), "string=%s", test5);
  is_int(4, count_tokens(test6, delim5678), "string=%s", test6);
  is_int(7, count_tokens(test7, delim5678), "string=%s", test7);
  is_int(0, count_tokens(test8, delim5678), "string=%s", test8);
  diag("delimiters for 9, 10, 11, 12=\"%s\"", delim9_15);
  is_int(4, count_tokens(test9, delim9_15), "string=%s", test9);
  is_int(6, count_tokens(test10, delim9_15), "string=%s", test10);
  is_int(0, count_tokens(test11, delim9_15), "string 11 is empty");
  is_int(1, count_tokens(test12, delim9_15), "string=%s", test12);
  is_int(0, count_tokens(NULL, delim9_15), "null instead of string.");
  is_int(0, count_tokens(test14, NULL), "null instead of tokens.");
  is_int(0, count_tokens(NULL, NULL), "null for both.");
  is_int(1, count_tokens(test16, delim16), "string=%s", test16);
  return EXIT_SUCCESS;
}


