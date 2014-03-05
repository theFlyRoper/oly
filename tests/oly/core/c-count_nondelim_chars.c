/* count_nondelim_chars test License GPL2+ {{{
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
#include "tests/tap/basic.h"
#include "src/messages/get_default_locale.c"

/* MAIN */
int
main( int argc, char **argv ){
  size_t         len       = 0;
  char            *program_name = argv[0];
  char            delim1234[] = ":", delim5678[] = ":|";
  char            test1[] = "6:chars", test2[] = ":ten:chars:!!",
                  test3[] = ":six:chr:",
                  test4[] = ":more:char:",
                  test5[] = "|wow:nine:oh|",
                  test6[] = "one|four:|:seven:|",
                  test7[] = ":lead:follow:",
                  test8[] = "::::||||",
                  test9[] = "";
 
  plan(9);
  diag("delimiters for 1 2 3 and 4=\"%s\"", delim1234);
  is_int(6, count_nondelim_chars(test1, delim1234), "string=%s", test1);
  is_int(10, count_nondelim_chars(test2, delim1234), "string=%s", test2);
  is_int(6, count_nondelim_chars(test3, delim1234), "string=%s", test3);
  is_int(8, count_nondelim_chars(test4, delim1234), "string=%s", test4);
  diag("delimiters for 5 6 7 and 8=\"%s\"", delim5678);
  is_int(9, count_nondelim_chars(test5, delim5678), "string=%s", test5);
  is_int(12, count_nondelim_chars(test6, delim5678), "string=%s", test6);
  is_int(10, count_nondelim_chars(test7, delim5678), "string=%s", test7);
  is_int(0, count_nondelim_chars(test8, delim5678), "string=%s", test8);
  is_int(0, count_nondelim_chars(test9, delim5678), "string=%s", test9);

  return EXIT_SUCCESS;
}


