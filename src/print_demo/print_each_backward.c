/*
 * file print_each_backward.c
 * Print each record forward.
 */

#include "oly/common.h"

#include <unicode/ustdio.h> 
#include <unicode/ustring.h>
#include <unicode/ubrk.h>

#include <stdio.h>
#include <stdlib.h>

#include "oly/output.h"

#ifdef OLYDEV
#include <assert.h>
#include "oly/oly_dev.h"
#endif /* OLYDEV */

/* Print each element in reverse order: */
void print_each_backward( UBreakIterator* boundary, UChar* str) {
  int32_t start;
  int32_t end = ubrk_last(boundary);
  for (start = ubrk_previous(boundary); start != UBRK_DONE;  end = start,
	 start = ubrk_previous(boundary)) {
    print_text_range( str, start, end );
  }
}

