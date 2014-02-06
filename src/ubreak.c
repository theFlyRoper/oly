/*
*******************************************************************************
*
*   Copyright (C) 2002, International Business Machines
*   Corporation and others.  All Rights Reserved.
*
*******************************************************************************
*/
#include <unicode/ustdio.h>
#include <unicode/ustring.h>
#include <unicode/ubrk.h>

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "loader.h"
#include "output.h"
#include "error.h"

U_CFUNC int c_main(void);

UFILE *u_stdout = NULL;   /* Unicode output file */
UFILE *u_stdin  = NULL;   /* Unicode input file */
UFILE *u_stderr = NULL;
const UChar *program_name = "Supadupa";

/* Creating and using text boundaries*/
int main(int argc, char **argv) {
  UBreakIterator  *boundary;
  char            c_string_to_examine[] = "\"Ada Bee\" \t,Clog,\"Lorem ipsum, dolor\",Aich";
  char            c_string_to_examine2[] = "9,\"Ada Bee\",Cee,\"Lorem ips\"";
  char            rules_file_name[] = "./supabreak.txt";
  UChar           string_to_examine[sizeof(c_string_to_examine)+1]; 
  UErrorCode      status=U_ZERO_ERROR;
  FILE            *u_rules=NULL;

  /* Initialize ICU */
  u_init(&status);
  if (U_FAILURE(status)) {
      fprintf(stderr, "%s: can not initialize ICU.  status = %s\n",
          argv[0], u_errorName(status));
      exit(1);
  }
  status = U_ZERO_ERROR;

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
  
  boundary = get_rules(rules_file_name, status);
  ubrk_setText(boundary, string_to_examine, u_strlen(string_to_examine), &status);

  /*print each sentence in forward and reverse order*/
  if (U_FAILURE(status)) {
    printf("ubrk_open error: %s\n", u_errorName(status));
    exit(1);
  }

  printf("\n----- test sequence 1, forward -----------\n"); 
  print_each_forward(boundary, string_to_examine);
  printf("\n----- test sequence 1, backward: ----------\n");
  print_each_backward(boundary, string_to_examine);
  ubrk_close(boundary);
  
  printf("\n\nExamining: %s\n", c_string_to_examine2);
  u_uastrcpy(string_to_examine, c_string_to_examine2);
  
  boundary = get_rules(rules_file_name, status);
  ubrk_setText(boundary, string_to_examine, u_strlen(string_to_examine), &status);
  
  printf("\n----- test sequence 2 -----------\n"); 
  print_each_forward(boundary, string_to_examine);
    
//  /*print each word in order*/
//  boundary = ubrk_open(UBRK_WORD, "en_us", string_to_examine,
//		       u_strlen(string_to_examine), &status);
//  printf("\n----- Word Boundaries, forward: -----------\n"); 
//  print_each_forward(boundary, string_to_examine);
//  printf("\n----- Word Boundaries, backward: ----------\n");
//  print_each_backward(boundary, string_to_examine);
//  /*print first element*/
//  printf("\n----- first: -------------\n");
//  print_first(boundary, string_to_examine);
//  /*print last element*/
//  printf("\n----- last: --------------\n");
//  print_last(boundary, string_to_examine);
//  /*print word at charpos 10 */
//  printf("\n----- at pos 10: ---------\n");
//  print_at(boundary, 10 , string_to_examine);
//    
  ubrk_close(boundary);

  printf("\nEnd of C boundary analysis\n");
  return 0;
}

