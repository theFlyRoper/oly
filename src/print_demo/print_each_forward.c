/*
 * file print_each_forward.c
 *
 * Print each record forward.
 */

#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif

#include <unicode/ustdio.h> 
#include <unicode/ustring.h>
#include <unicode/ubrk.h>

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "output.h"

#ifdef OLYDEV
#include <assert.h>
#include "oly_dev.h"
#endif /* OLYDEV */

/* Print each element in order: */
void 
print_each_forward( UBreakIterator* boundary, UChar* str) {
  int32_t end;
  int32_t start = ubrk_first(boundary);
  for (end = ubrk_next(boundary); end != UBRK_DONE; start = end, end =
	 ubrk_next(boundary)) {
    print_text_range(str, start, end );
  }
}

