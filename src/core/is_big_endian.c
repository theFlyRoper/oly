/* a little check on endianness 
 * see here: http://stackoverflow.com/questions/1001307/detecting-endianness-programmatically-in-a-c-program/1001373#1001373
 */

#include "oly/common.h"
#include "oly/core.h"

#include "stdint.h"

int
is_big_endian (void) {
  union {
    uint32_t  intpart;
    char      *charpart;
  } bigcheck = {0xABCDEF01};
  return (*bigcheck.charpart == 0xAB);
}
