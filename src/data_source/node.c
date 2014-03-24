/* node.c -- collection data source functions. License GPL2+ {{{
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

OlyNode *
new_oly_ds_node( OlyStatus *status )
{
    OlyNode        *new_node = NULL;
    UErrorCode      u_status = U_ZERO_ERROR;
    if (*status != OLY_OKAY)
    {
        return NULL;
    }
    new_node = omalloc(sizeof(OlyNode));
    if ( new_node == NULL )
    {
        *status = OLY_ERR_NOMEM;
        return NULL;
    }
    
    new_node->current_level     = 0;
    new_node->vt                = OLY_NODE_VALUE_TYPE_UNSET;
    new_node->tuple             = 0;
    new_node->key               = NULL;
    new_node->parent_node       = NULL;
    
    (new_node->value).int_value = 0;
    
    return new_node;
}

void close_oly_ds_node( OlyNode *node )
{
    OlyStatus status = OLY_OKAY;

    if (((node->value).string_value) != NULL)
    {
        free((node->value).string_value);
    }
    if (node->key != NULL)
    {
        free(node->key);
    }
    if (node != NULL)
    {
        free(node);
    }
    return;
}

OlyStatus  descend_one_level( OlyNode **node )
{
    OlyStatus  status = OLY_OKAY;
    OlyNode *next_node = NULL;
    if (((*node)->current_level + 1) > MAX_NODE_DEPTH)
    {
        status = OLY_ERR_NODES_TOO_DEEP;
        return status;
    }
    next_node = new_oly_ds_node( &status );
    next_node->parent_node = *node;
    next_node->current_level = ((*node)->current_level + 1);
    *node = next_node;
    return status;
}

OlyStatus  ascend_one_level( OlyNode **node )
{
    OlyStatus  status = OLY_OKAY;
    OlyNode *next_node = NULL;
    if (((*node)->current_level - 1) < 0)
    {
        status = OLY_ERR_NODES_TOO_SHALLOW;
        return status;
    }
    next_node = (*node)->parent_node ;
    (*node) = next_node;
    return status;
}

/* key is not required.  If key is null, advance node assumes a tuple. 
 * key is copied into the charset translation buffer.
 * Before copying, key is checked for length.  Oly requires that the key be at most 
 * 1024 unicode characters long, per YAML.  It is just simpler. */

extern OlyStatus  advance_node(OlyNode *node, char *key );
extern OlyStatus  set_node_string_value ( OlyNode *node, char *value);
extern OlyStatus  set_node_float_value  ( OlyNode *node, const double value);
extern OlyStatus  set_node_int_value    ( OlyNode *node, const long value);

extern OChar     *get_node_key(OlyNode *node, OlyStatus *status);
extern OChar     *get_node_string_value(OlyNode *node, OlyStatus *status);
extern double     get_node_float_value(OlyNode *node, OlyStatus *status);
extern long       get_node_int_value(OlyNode *node, OlyStatus *status);



