/* node_internal.h -- Internal node functions, not for use by data sources. License GPL2+ {{{
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

#include "oly/node.h"

#ifndef OLY_NODE_INTERNAL_H
#define OLY_NODE_INTERNAL_H 1

#define NODE_IO(arg) \
    OlyStatus lo_set_##arg ( OlyNode **node, OChar * arg , uint16_t length );
node_interface
#undef NODE_IO
#define NODE_IO(arg) \
    OlyStatus lo_get_##arg ( OlyNode **node, OChar ** arg , uint16_t *length );
node_interface
#undef NODE_IO

struct oly_node_queue_struct;
typedef struct oly_node_queue_struct OlyNodeQueue;

/* OlyNodeQueue functions */
OlyStatus open_node_queue(OlyNodeQueue **new_queue);
void close_node_queue(OlyNodeQueue *free_me);
OlyStatus enqueue_to_node_queue( OlyNodeQueue *q, OlyNode *n );
OlyStatus dequeue_from_node_queue(OlyNodeQueue *q, OlyNode **node_out);

#endif /* OLY_NODE_INTERNAL_H */

