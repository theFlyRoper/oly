/* node.h -- node abstract type definitions. License GPL2+ {{{
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

#ifndef OLY_NODE_H
#define OLY_NODE_H 1

#include "oly/common.h"
#include <math.h>

BEGIN_C_DECLS

typedef enum oly_node_type_enum {
    OLY_NODE_VALUE_TYPE_UNSET,
    OLY_NODE_VALUE_TYPE_TOP,
    OLY_NODE_VALUE_TYPE_MAP,
    OLY_NODE_VALUE_TYPE_SEQUENCE,
    OLY_NODE_VALUE_SCALAR_STRING,
    OLY_NODE_VALUE_SCALAR_FLOAT,
    OLY_NODE_VALUE_SCALAR_INT
} OlyNodeValueType;

struct oly_data_source_node_struct;
typedef struct oly_data_source_node_struct OlyNode;

/* dispatch function for filled node buffers. Any data source will have one for incoming and one for outgoing. May be used elsewhere later. */
typedef OlyStatus (* OlyNodeDispatch)(OlyNode **source, OlyNode **dest, size_t node_count); 

extern OlyNode *new_oly_ds_node( OlyStatus *status );
extern void     close_oly_ds_node( OlyNode *node );
/* key is not required.  If key is null, advance node assumes a tuple. 
 * key is copied into the charset translation buffer.
 * Before copying, key is checked for length.  Oly requires that the key be at most 
 * 1024 unicode characters long, per YAML.  It is just simpler. */
extern OlyStatus  descend_one_level( OlyNode **node );
extern OlyStatus  ascend_one_level ( OlyNode **node );
extern OChar     *get_node_key(OlyNode *node, OlyStatus *status);


/* scalar value functions */
extern OlyStatus  set_node_string_value(OlyNode *node, char *value);
extern OlyStatus  set_node_float_value(OlyNode *node, const double value);
extern OlyStatus  set_node_int_value(OlyNode *node, const long value);

extern OChar     *get_node_string_value(OlyNode *node, OlyStatus *status);
extern double     get_node_float_value(OlyNode *node, OlyStatus *status);
extern long       get_node_int_value(OlyNode *node, OlyStatus *status);

END_C_DECLS

#endif /* OLY_NODE_H */

