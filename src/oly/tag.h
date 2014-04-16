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
#include "oly/olytypes.h"
#include "oly/hash.h"

struct oly_node_value_struct;
typedef struct oly_node_value_struct OlyNodeValue;

/* basic types.  Oly guarantees these will be available. */
typedef enum oly_tag_type_enum {
    OLY_TAG_TYPE_UNSET = 0,
    OLY_TAG_MIN = 0,
    OLY_TAG_TYPE_MAP = 1,
    OLY_TAG_TYPE_SEQUENCE = 2,
    OLY_TAG_ALIAS = 3,
    OLY_TAG_SCALAR_NULL = 4,
    OLY_TAG_SCALAR_BOOL = 5,
    OLY_TAG_SCALAR_INT = 6,
    OLY_TAG_SCALAR_UINT = 7,
    OLY_TAG_SCALAR_FLOAT = 8,
    OLY_TAG_SCALAR_STRING = 9,
    OLY_TAG_FILE_TEXT = 10,
    OLY_TAG_FILE_BINARY = 11,
    OLY_TAG_MAX = 11
} OlyTagType;

typedef struct oly_tag_struct
{
  OChar *name;
  int (*verify)(OlyNodeValue *);
  OlyStatus (* convert)(OlyNodeValue *input, OlyNodeValue **output);
  OlyTagType internal_type;
} OlyTag;

OlyStatus infer_simple_tag( OChar *scalar, OlyTagType *type_out );
OlyStatus load_tag_table( OlyTag **t, OlyHashTable **tag );

#endif /* OLY_TAG_H */


