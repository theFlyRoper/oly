/*
 * file print_first.c
 *
 * Print first from each record.
 */

#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif

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

/* Print first element */
void print_first(UBreakIterator* boundary, UChar* str) {
  int32_t end;
  int32_t start = ubrk_first(boundary);
  end = ubrk_next(boundary);
  print_text_range( str, start, end );
}

