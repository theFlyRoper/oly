/* nchar_bitand.c: N chars of s1 are anded with N chars of s2 and written to out. */

#include <assert.h>
#include "oly/oly_hash.h"
#include "oly/state.h"
oly_status nchar_bitand (const char *s1, const char *s2, char *out, size_t len) {
  oly_status status = OLY_OKAY;
  size_t     i = 0;
 
  assert( (s1 != NULL) && (s2 != NULL) && (out != NULL) 
      && (s1[len] != NULL) && (s2[len] != NULL) && (out[len] != NULL) && (len > 0) );
  for (i = 0; i < len; i++) {
    out[i] = (s1[i] & s2[i]);
  }
  return status;
}

