/*
 * file print_text_range.c
 *
 * show the text in this range.  From ubreak.c
 */

#ifdef HAVE_CONFIG_H
#  include "liboly/olyconf.h"
#endif

#include <unicode/ustdio.h> 
#include <unicode/ustring.h>
#include <unicode/ubrk.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef DEVMODE
#include <assert.h>
#include "oly_dev.h"
#endif /* DEVMODE */

void 
print_text_range(UChar* str, int32_t start, int32_t end)
{
  char    charBuf[1000];
  UChar   savedEndChar;

  savedEndChar = str[end];
  str[end] = 0;
  u_austrncpy(charBuf, str+start, sizeof(charBuf)-1);
  charBuf[sizeof(charBuf)-1]=0;
  printf("string[%2d..%2d] \"%s\"\n", start, end-1, charBuf); 
  str[end] = savedEndChar;
}
