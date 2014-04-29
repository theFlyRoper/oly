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
#include "oly/tag.h"
#include <stdbool.h>
#include <math.h>

BEGIN_C_DECLS

/* OlyNode definition is in tag.h */
struct oly_node_queue_struct;
typedef struct oly_node_queue_struct OlyNodeQueue;

/* OlyNodeQueue functions */
OlyStatus open_node_queue(OlyNodeQueue **new_queue);
void close_node_queue(OlyNodeQueue *free_me);
OlyStatus enqueue_to_node_queue( OlyNodeQueue *q, OlyNode *n );
OlyStatus dequeue_from_node_queue(OlyNodeQueue *q, OlyNode **node_out);

OlyStatus set_node_node_id(OlyNode *node, int64_t node_id);
OlyStatus copy_node(const OlyNode *source, OlyNode *dest);
OlyStatus reset_node( OlyNode *node );

/* node value functions */
OlyStatus set_node_value(OlyNode *node, void *value, OlyTagType type);
OlyStatus set_node_string_value(OlyNode *output, const OChar *value);
OlyStatus set_node_key(OlyNode *output, const OChar *key_in);

OlyStatus new_node_value( OlyNodeValue **new_node );
OlyStatus new_oly_node( OlyNode **new_node );
void close_oly_ds_node(OlyNode *node);
void print_node( OlyNode *n );

OlyStatus node_has_key(OlyNode *node);
OlyStatus set_node_has_key(OlyNode *node);
OlyStatus unset_node_has_key(OlyNode *node);

OlyStatus get_node_parent( const OlyNode *node, OlyNode **parent);
OlyStatus get_node_string_value(OlyNode *node, OChar **value_out);
OlyStatus get_node_key(OlyNode *node, OChar **key_out);
OlyStatus get_node_node_id(OlyNode *node, int64_t *node_id_out );
OlyStatus push_node(OlyNode **stack, OlyNode *node );
OlyStatus pop_node( OlyNode **stack, OlyNode **node_out );
OlyStatus descend_one_level( OlyNode *node );
OlyStatus ascend_one_level ( OlyNode *node );


END_C_DECLS

#endif /* OLY_NODE_H */

