/* * test for oly_getocharArgs *
 */
#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif

#include "oly/common.h"

#include <unicode/ustdio.h>
#include <unicode/ustring.h>

#include <string.h>
#include <stdio.h>

#include "oly/core.h"
#include "oly/state.h"

int
main( int argc, char **argv ){
  size_t          i = 0;
  char            *locale           = NULL;
  UErrorCode      u_status  = U_ZERO_ERROR; 
  ochar           **result, *curr;
  oly_status      status;

  u_init(&u_status);
  init_io(locale, NULL);

  if (U_FAILURE(u_status)) {
    printf("Could not open! status: %s\n", u_errorName(u_status));
    return EXIT_FAILURE;
  }
  status = oly_getocharArgs(&result, argv, argc); 
  if (status != OLY_OKAY) {
    printf("Status: %i\n", status);
    return EXIT_FAILURE;
  }
  for (i = 1;  ((curr = result[i]) != NULL);  i++) {
    curr = result[i];
    u_fprintf(u_stdout, "%S!", curr);
  }
  u_fprintf(u_stdout, "\n");
  return EXIT_SUCCESS;
}
