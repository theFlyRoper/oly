/* collection_ds.h -- collection data source definitions. License GPL2+ {{{
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

#ifndef OLY_COLLECTION_DS_H
#define OLY_COLLECTION_DS_H 1

#include "oly/data_source.h"
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
} OlyDSNodeValueType;

struct oly_data_source_node_struct;
typedef struct oly_data_source_node_struct OlyDSNode;

extern OlyDSNode *new_oly_ds_node( OlyStatus *status );
extern void       close_oly_ds_node( OlyDSNode *node );
/* key is not required.  If key is null, advance node assumes a tuple. 
 * key is copied into the charset translation buffer.
 * Before copying, key is checked for length.  Oly requires that the key be at most 
 * 1024 unicode characters long, per YAML.  It is just simpler.
 */
extern OlyStatus  advance_node(OlyDSNode *node, char *key );
extern OlyStatus  set_node_string_value(OlyDSNode *node, char *value);
extern OlyStatus  set_node_float_value(OlyDSNode *node, const double value);
extern OlyStatus  set_node_int_value(OlyDSNode *node, const long value);
extern OlyStatus  descend_one_level( OlyDSNode **node );
extern OlyStatus  ascend_one_level ( OlyDSNode **node );


extern OChar     *get_node_key(OlyDSNode *node, OlyStatus *status);
extern OChar     *get_node_string_value(OlyDSNode *node, OlyStatus *status);
extern double     get_node_float_value(OlyDSNode *node, OlyStatus *status);
extern long       get_node_int_value(OlyDSNode *node, OlyStatus *status);

END_C_DECLS

#endif /* OLY_COLLECTION_DS_H */

