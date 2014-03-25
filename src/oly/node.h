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

union oly_node_value_union;
typedef union oly_node_value_union OlyNodeValue;

typedef enum oly_node_value_type_enum {
    OLY_NODE_VALUE_TYPE_UNSET = 0,
    OLY_NODE_VALUE_MIN = 0,
    OLY_NODE_VALUE_TYPE_MAP = 1,
    OLY_NODE_VALUE_TYPE_SEQUENCE = 2,
    OLY_NODE_VALUE_SCALAR_STRING = 3,
    OLY_NODE_VALUE_SCALAR_FLOAT = 4,
    OLY_NODE_VALUE_SCALAR_INT = 5,
    OLY_NODE_VALUE_MAX = 5
} OlyNodeValueType;

struct oly_data_source_node_struct;
typedef struct oly_data_source_node_struct OlyNode;

extern void reset_node( OlyNode *node );
extern OlyNode *new_oly_ds_node( OlyStatus *status );
extern OlyNodeValue *new_node_value( void );
extern void     close_oly_ds_node( OlyNode *node );
extern OlyStatus  descend_one_level( OlyNode **node );
extern OlyStatus  ascend_one_level ( OlyNode **node );
extern OChar *get_node_key(OlyNode *node, OlyStatus *status);
extern unsigned char node_has_key(OlyNode *node);
extern OlyStatus unset_node_has_key(OlyNode *node);
extern OlyStatus set_node_tuple(OlyNode *node, int64_t tuple);
extern int64_t get_node_tuple(OlyNode *node, OlyStatus *status);
extern int64_t get_parent_tuple(OlyNode *node, OlyStatus *status);
extern OlyStatus copy_node(OlyNode *source, OlyNode *dest);

/* node value functions */
extern OlyStatus set_node_value(OlyNode *node, void *value, OlyNodeValueType type);
extern OlyStatus set_node_string_value(OlyNodeValue *output, const OChar *value);

END_C_DECLS

#endif /* OLY_NODE_H */

