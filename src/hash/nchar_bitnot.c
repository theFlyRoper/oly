/* nchar_bitnot.c: not N chars from s into not_s */

#include <assert.h>
#include "oly/oly_hash.h"
#include "oly/state.h"
oly_status nchar_bitnot(const char *s,  char *not_s, size_t len){
  oly_status status = OLY_OKAY;
  size_t     i = 0;
 
  assert( (s != NULL) && (not_s != NULL) && (s[len] != NULL) 
      && (not_s[len] != NULL) && (len > 0) );;
  for (i = 0; i < len; i++) {
    not_s[i] = ~s[i];
  }
  return status;
}

