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
#include "oly/core.h"

/* MAIN */
int
main( int argc, char **argv ){
  const size_t    test1_size = 4;
  size_t          i = 0;
  char            *program_name = argv[0];
  char            delim1234[] = ":", delim5678[] = ":|";
  char            test1[] = ":ten:chars:!!:3120",
                  **result,
                  *output[] = {
                    "ten", "chars", "!!", "3120" } ;
  oly_status      status;

 
  plan(test1_size);
  result = token_str_to_array(test1, delim1234, &status);
  for ( i=0; i < test1_size; i++ ) {
    is_string(result[i], output[i], "result[%i](%s) = output[%i](%s)", i, result[i], i, output[i] );
  }
  return EXIT_SUCCESS;
}

