/*
 * file print_each_backward.c
 * Print each record forward.
 */

#ifdef HAVE_CONFIG_H
#  include "liboly/olyconf.h"
#endif

#include <unicode/ustdio.h> 
#include <unicode/ustring.h>
#include <unicode/ubrk.h>

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "output.h"

#ifdef DEVMODE
#include <assert.h>
#include "oly_dev.h"
#endif /* DEVMODE */

/* Print each element in reverse order: */
void print_each_backward( UBreakIterator* boundary, UChar* str) {
  int32_t start;
  int32_t end = ubrk_last(boundary);
  for (start = ubrk_previous(boundary); start != UBRK_DONE;  end = start,
	 start = ubrk_previous(boundary)) {
    print_text_range( str, start, end );
  }
}

