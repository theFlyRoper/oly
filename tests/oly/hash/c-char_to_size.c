/* char_to_size test License GPL2+ {{{
 * Copyright (C) 2014 Oly Project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * }}} */

#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif

#include "oly/common.h"

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "oly/state.h"
#include "oly/hash.h"
#include "oly/globals.h"
#include "tests/tap/basic.h"
#include "pvt_core.h"

/* HashReturn Init(hashState *state, int hashbitlen);
HashReturn Update(hashState *state, const BitSequence *data, 
    DataLength databitlen);
HashReturn Final(hashState *state, BitSequence *hashval);
HashReturn Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval); */
typedef union oly_hash_union {
  size_t        sval[SIZE_HASH];
  unsigned char cval[CHAR_HASH];
} oly_hash;


int
main (void){
    char            *locale = (char *)"root",  *charset = NULL;
    oly_hash    test1 = {{0xEFBEADBAED0BEDFE}}, 
                test2 = {{0xEDEDDE00EDADBE00}},
                test3 = {{0xED0BEDFEADDEEDDE}};
    size_t      result1 = {0xFEED0BEDBAADBEEF}, 
                result2 = {0x00BEADED00DEEDED}, 
                result3 = {0xDEEDDEADFEED0BED}, output_val = 0;
    OlyStatus  status;
    diag("TODO: rearrange this to add a bigendian chunk.");
    plan(3);
    status = char_to_size(test1.cval,&output_val);
    is_hex(result1, output_val, "%016lX: Probably mad cow disease.", output_val);
    status = char_to_size(test2.cval,&output_val);
    is_hex(result2, output_val, "%016lX: That was how he rolled, man.", output_val);
    status = char_to_size(test3.cval,&output_val);
    is_hex(result3, output_val, "%016lX: A feedbed without deeds is dead.", output_val);
    return EXIT_SUCCESS;
  
}

