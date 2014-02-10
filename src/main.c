/* main for csv importer. License GPL2+ {{{
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

#include <unicode/ustdio.h>
#include <unicode/ustring.h>
#include <unicode/ubrk.h>
#include <unicode/udata.h>
#include <unicode/ures.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <assert.h>

#include "common.h"
#include "output.h"
#include "error.h"
#include "list.h"
#include "syntax.h"
#include "oly.h"
#include "builtin.h"
#include "loader.h"

UFILE *u_stdout = NULL;   /* Unicode output */
UFILE *u_stdin  = NULL;   /* Unicode input */
UFILE *u_stderr = NULL;   /* Unicode error output */
const UChar *program_name;

/* MAIN */
int
main( int argc, char **argv ){
  Oly             *oly      = oly_new ();
  int32_t         len,optc  = 0;
  UErrorCode      u_status  = U_ZERO_ERROR; /* Unicode u_status code */
  Oly_Status      o_status  = OLY_OKAY;
  UBreakIterator  *boundary;
  char            rules_file_name[] = "./tests/supabreak.txt";
  UChar           line[BUFSIZ];     /* main buffer */
  UBool           displayUsage      = FALSE;
  char            *optionError      = NULL;     
  char            *locale           = NULL;
  UResourceBundle *OlyResources;
  int             i;
  UChar           *resString;

  char            c_line[] = "Rusty,\"Block, Head\", Blomster, \"3.1415,92,9\"";
  char            c_line2[] = "\"Lorem, ipsum\",Blomster,\"1.41\",Yorgle";
  atexit (close_oly);
  program_name      = argv[0];
  
  /* u_setDataDirectory tells ICU where to look for custom app data.  It is not needed
   * for the internal app data for ICU, which lives in a shared library. */

  u_setDataDirectory("./resources");
  init_all(oly, locale);
  
  printf("Examining: %s\n", c_line);
  u_uastrcpy(line, c_line);

  boundary = get_rules(rules_file_name, u_status);
  ubrk_setText(boundary, line, u_strlen(line), &u_status);

  /*print each sentence in forward and reverse order*/
  if (U_FAILURE(u_status)) {
    printf("ubrk_open error: %s\n", u_errorName(u_status));
    exit(1);
  }

  printf("\n----- test sequence 1, forward -----------\n"); 
  print_each_forward(boundary, line);
  printf("\n----- test sequence 1, backward: ----------\n");
  print_each_backward(boundary, line);
  ubrk_close(boundary);

  printf("\n\nExamining: %s\n", c_line2);
  u_uastrcpy(line, c_line2);

  boundary = get_rules(rules_file_name, u_status);
  ubrk_setText(boundary, line, u_strlen(line), &u_status);

  printf("\n----- test sequence 2 -----------\n"); 
  print_each_forward(boundary, line);
    
/*  boundary = ubrk_open(UBRK_WORD, "en_us", line,
		       u_strlen(line), &u_status);
  printf("\n----- Word Boundaries, forward: -----------\n"); 
  print_each_forward(boundary, line);
  printf("\n----- Word Boundaries, backward: ----------\n");
  print_each_backward(boundary, line);
  printf("\n----- first: -------------\n");
  print_first(boundary, line);
  printf("\n----- last: --------------\n");
  print_last(boundary, line);
  printf("\n----- at pos 10: ---------\n");
  print_at(boundary, 10 , line); */
  
  ubrk_close(boundary);

  printf("\nEnd of C boundary analysis\n");
  
#ifdef OLYDEV

#endif /* OLYDEV */
  
  return u_status;
}


