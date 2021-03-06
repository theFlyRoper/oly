/* node_internal.c -- internal node functions, use for working with nodes inside Oly.  License GPL2+ {{{
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

#include "node/pvt_node.h"

#define NODE_IO(arg) \
    OlyStatus lo_set_##arg (OlyNode **node, OChar *arg, uint16_t length) \
{ OlyStatus status = OLY_OKAY; \
    if ((arg == NULL) || (node == NULL) || ( length == 0 )) \
        { status = OLY_ERR_BADARG ; } \
    ((*node)->arg).ochar_str = arg ; \
    (*node)->arg##_length = length; \
    return status; }
node_interface
#undef NODE_IO
#define NODE_IO(arg) \
    OlyStatus lo_get_##arg (OlyNode **node, OChar **arg, uint16_t *length) \
{ OlyStatus status = OLY_OKAY; \
    if ((arg == NULL) || (node == NULL) || (length == NULL)) \
        { status = OLY_ERR_BADARG ; } \
    *length = (*node)->arg##_length ; \
    (*arg ) = ((*node)->arg).ochar_str  ; \
    return status; }
node_interface
#undef NODE_IO

