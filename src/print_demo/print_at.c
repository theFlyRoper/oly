/*
 * file print_at.c
 * Print at specified point.
 */

#include <unicode/ustdio.h> 
#include <unicode/ustring.h>
#include <unicode/ubrk.h>

#include <stdio.h>
#include <stdlib.h>

#include "oly/common.h"
#include "oly/output.h"

#ifdef OLYDEV
#include <assert.h>
#include "oly/oly_dev.h"
#endif /* OLYDEV */

/* Print the element at a specified position */
void print_at(UBreakIterator* boundary, int32_t pos , UChar* str) {
  int32_t start;
  int32_t end = ubrk_following(boundary, pos);
  start = ubrk_previous(boundary);
  print_text_range(str, start, end );
}

