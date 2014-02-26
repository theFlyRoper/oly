#include "oly/oly_hash.h"

oly_status 
oly_hash( const bit_sequence *data, data_length len, bit_sequence *hash ) 
{
  if (Hash(OLY_HASH_SIZE, data, len, hash) != SUCCESS) {
    return OLY_ERR_UNSPECIFIED;
  };
  return OLY_OKAY;
}

