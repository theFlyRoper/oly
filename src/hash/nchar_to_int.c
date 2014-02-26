/* nchar_to_int.c: convert N chars into an array of ints */

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include "oly/oly_hash.h"
#include "oly/state.h"

oly_status str4_to_int(const char *c, int *res){
  /* converts 4 chars into a single int. */
  int       combiner = 0;
  size_t    i = sizeof(int);
  *res = 0;
  for (i = sizeof(int); (i > 0); i -= sizeof(char)) {
    *res <<= CHAR_BIT;
    printf("char value: %u res value: %u\n",(unsigned char)*(c + (i-1)), *res);
    *res |= (unsigned char)c[(i-1)];
  }
  return OLY_OKAY;
}

void print_result(const char *c){
  int       i=0, limit = (OLY_HASH_SIZE/CHAR_BIT);
  for (i = 0; (i <limit ); i += sizeof(char)) {
    printf("%02x", (unsigned char)c[i]);
  }
  printf("\n");
}
/* 
void print_result(const char *c){
  int       i=0, limit = (OLY_HASH_SIZE/CHAR_BIT);
  for (i = limit; (i > 0); i -= sizeof(char)) {
    printf("%02x", (unsigned char)c[(i-1)]);
  }
  printf("\n");
}
counting down version */

