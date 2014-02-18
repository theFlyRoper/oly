/* * test for oly_getOCharArgs *
 */
#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif

#include "common.h"

#include <unicode/ustdio.h>
#include <unicode/ustring.h>

#include <string.h>
#include <stdio.h>

#include "error.h"
#include "olyio.h"

int
main( int argc, char **argv ){
  size_t          i = 0;
  char            *locale           = NULL;
  UErrorCode      u_status  = U_ZERO_ERROR; 
  OChar           **result, *curr;
  Oly_Status      status;

  u_init(&u_status);
  init_io(locale);

  if (U_FAILURE(u_status)) {
    printf("Could not open! status: %s\n", u_errorName(u_status));
  }
  status = oly_getOCharArgs(&result, argv, argc); 
  for (i = 0;  ((curr = result[i]) != NULL);  i++) {
    curr = result[i];
    u_fprintf(u_stdout, "NumItems: %i\n", i);
    u_fprintf(u_stdout, "curr: %S\n", curr);
  }
  return EXIT_SUCCESS;
}
