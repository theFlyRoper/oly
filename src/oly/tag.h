/* tag.h -- tag abstract type definitions. License GPL2+ {{{
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
 *
 * }}} */

#ifndef OLY_TAG_H
#define OLY_TAG_H 1

#include "oly/common.h"
#include <stdbool.h>
#include "oly/olytypes.h"
#include "oly/node.h"
#include "oly/hash.h"

BEGIN_C_DECLS

/* OlyNode defined in pvt_node.h */
struct oly_node_struct;
typedef struct oly_node_struct OlyNode;

struct oly_node_value_struct;
typedef struct oly_node_value_struct OlyNodeValue;

/* basic types.  Oly guarantees these will be available. Any further types
 * are descended from OLY_TAG_COMPLEX_MAP and OLY_TAG_COMPLEX_SEQUENCE, and contain
 * combinations of the basic types. */
typedef enum oly_tag_type_enum {
    OLY_TAG_SCALAR_NULL,
    OLY_TAG_SCALAR_BOOL,
    OLY_TAG_SCALAR_INT,
    OLY_TAG_SCALAR_UINT,
    OLY_TAG_SCALAR_FLOAT,
    OLY_TAG_SCALAR_INFINITY,
    OLY_TAG_SCALAR_NOT_A_NUMBER,
    OLY_TAG_SCALAR_STRING,
    OLY_TAG_ALIAS,
    OLY_TAG_FILE_TEXT,
    OLY_TAG_FILE_BINARY,
    OLY_TAG_COMPLEX_MAP,
    OLY_TAG_COMPLEX_SEQUENCE,
    OLY_TAG_TYPE_UNSET,
    OLY_TAG_MAX
} OlyTagType;

/* TODO: long, unsigned long and double are not pointers and may align differently.
 * This union should be space efficient above all, so figure out how to ensure
 * these data types all line up, ideally without a cast.  On 64 bit linux,
 * long, unsigned long and double work nicely.  */

struct oly_node_value_struct
{

    union oly_node_value_union 
    {
        OlyNode             **collection_value;
        OChar                *string_value;
        FILE                 *large_binary_file;
        OFILE                *large_text_file;
        double                float_value;
        long                  int_value;
        unsigned long         uint_value;
        bool                  bool_value;
    } value;
    OlyTagType   type;
};

typedef struct oly_tag_struct
{
  OlyStatus (* check)(OChar *input);
  OlyStatus (* import)(OChar *input, OlyNodeValue *output);
  OlyTagType internal_type;
} OlyTag;

OlyStatus infer_simple_tag( OChar *scalar, OlyTagType *type_out );
OlyStatus convert_tag( OChar *scalar, OlyTagType *type_out );
END_C_DECLS

#endif /* OLY_TAG_H */


