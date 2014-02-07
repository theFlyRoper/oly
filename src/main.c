/* main for csv importer. License GPL2+ {{{
 * Copyright (C) 2012 Oly Project
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
 *
 * }}} */

#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif

#include <unicode/ustdio.h> 
#include <unicode/ustring.h>
#include <unicode/ubrk.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>

#include "common.h"
#include "loader.h"
#include "output.h"
#include "error.h"
#include "oly.h"

#ifdef OLYDEV
#include <assert.h>
#include "oly_dev.h"
#endif /* OLYDEV */

UFILE *u_stdout = NULL;   /* Unicode output file */
UFILE *u_stdin  = NULL;   /* Unicode input file */
UFILE *u_stderr = NULL;

/* STATIC FUNCTION FORWARD DECLARATIONS */

static void print_help (void);
static void print_version (void);
static void close_oly (void);
static void set_program_name (const char *path);

/* The following function(s) are
 * adapted from the Secure Programming 
 * Cookbook, John Viega and Matt Messier.
 * 2003, O'Reilly Press.
 */

static int cleanenv();

#include "inline_h/close_oly-inl.h"
#include "inline_h/print_help-inl.h"
#include "inline_h/print_version-inl.h"
#include "inline_h/cleanenv-inl.h"
#include "inline_h/set_program_name-inl.h"

/* MAIN */
int
main( int argc, char *argv[] ){

  int32_t         len,optc  = 0;      /* current line length */
  UErrorCode      u_status  = U_ZERO_ERROR; /* Unicode u_status code */
  UBreakIterator  *boundary;
  char            c_string_to_examine[] = "\"Ada Bee\" \t,Clog,\"Lorem ipsum, dolor\",Aich";
  char            c_string_to_examine2[] = "9,\"Ada Bee\",Cee,\"Lorem ips\"";
  char            rules_file_name[] = "./tests/supabreak.txt";
  UChar           string_to_examine[sizeof(c_string_to_examine)+1]; 
  UChar           line[BUFSIZ];      /* main buffer */

  atexit (close_oly);
  
  u_status = cleanenv();

  program_name = argv[0];

/* Initialize ICU */
  u_init(&u_status);
  if (U_FAILURE(u_status)) {
      fprintf(stderr, "%s: can not initialize ICU.  u_status = %s\n",
          argv[0], u_errorName(u_status));
      exit(1);
  }
  u_status = U_ZERO_ERROR;

  if ( u_stderr == NULL )
      u_stderr=u_finit(stderr, NULL /*locale*/,  NULL /*codepage */);
    
  if( u_stdout == NULL ) {
      u_stdout = u_finit(stdout, NULL /*locale*/,  NULL /*codepage */);
      if(!u_stdout) {
          fprintf(stderr, "Could not initialize (finit()) over stdout! \n");
          return 1;
      }
  };
  if(u_stdin == NULL) {
      u_stdin = u_finit(stdin, NULL, NULL);
      if(!u_stdin) {
          fprintf(stderr, 
              "%s: Could not initiate input %s!\n",
              program_name, program_name);
          return 1;
      }
    };

  printf("\n\n"
    "C Boundary Analysis\n"
    "-------------------\n\n");

  printf("Examining: %s\n", c_string_to_examine);
  u_uastrcpy(string_to_examine, c_string_to_examine);

  boundary = get_rules(rules_file_name, u_status);
  ubrk_setText(boundary, string_to_examine, u_strlen(string_to_examine), &u_status);

  /*print each sentence in forward and reverse order*/
  if (U_FAILURE(u_status)) {
    printf("ubrk_open error: %s\n", u_errorName(u_status));
    exit(1);
  }

  printf("\n----- test sequence 1, forward -----------\n"); 
  print_each_forward(boundary, string_to_examine);
  printf("\n----- test sequence 1, backward: ----------\n");
  print_each_backward(boundary, string_to_examine);
  ubrk_close(boundary);

  printf("\n\nExamining: %s\n", c_string_to_examine2);
  u_uastrcpy(string_to_examine, c_string_to_examine2);

  boundary = get_rules(rules_file_name, u_status);
  ubrk_setText(boundary, string_to_examine, u_strlen(string_to_examine), &u_status);

  printf("\n----- test sequence 2 -----------\n"); 
  print_each_forward(boundary, string_to_examine);
    
/*  boundary = ubrk_open(UBRK_WORD, "en_us", string_to_examine,
		       u_strlen(string_to_examine), &u_status);
  printf("\n----- Word Boundaries, forward: -----------\n"); 
  print_each_forward(boundary, string_to_examine);
  printf("\n----- Word Boundaries, backward: ----------\n");
  print_each_backward(boundary, string_to_examine);
  printf("\n----- first: -------------\n");
  print_first(boundary, string_to_examine);
  printf("\n----- last: --------------\n");
  print_last(boundary, string_to_examine);
  printf("\n----- at pos 10: ---------\n");
  print_at(boundary, 10 , string_to_examine); */
  
  ubrk_close(boundary);

  printf("\nEnd of C boundary analysis\n");
  
#ifdef OLYDEV

#endif /* OLYDEV */
  
  return u_status;
}


