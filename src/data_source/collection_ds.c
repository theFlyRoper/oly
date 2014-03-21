/* collection_ds.c -- collection data source functions. License GPL2+ {{{
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

#include "data_source/pvt_collection_ds.h"

OlyDSNode *
new_oly_ds_node( OlyDataSource *ds, Oly OlyStatus *status )
{
    OlyDSNode       *new_node = NULL;
    UErrorCode      u_status = U_ZERO_ERROR;
    if (*status != OLY_OKAY)
    {
        return NULL;
    }
    new_node = omalloc(sizeof(OlyDSNode));
    if ( new_node == NULL )
    {
        *status = OLY_ERR_NOMEM;
        return NULL;
    }
    
    new_node->current_level     = 0;
    new_node->vt                = OLY_NODE_VALUE_TYPE_UNSET;
    new_node->parent_vt         = OLY_NODE_VALUE_TYPE_TOP;
    new_node->tuple             = 0;
    new_node->key               = NULL;
    new_node->parent_node       = NULL;
    new_node->data_source       = ds;
    new_node->buffer_size       = 0;
    new_node->ochar_buffer      = ds->ochar_buffer;
    new_node->char_buffer       = ds->char_buffer;
    
    (new_node->value).int_value = 0;
    
    return new_node;
}

void close_oly_ds_node( OlyDSNode *node )
{
    OlyStatus status = OLY_OKAY;

    if (((node->value).string_value) != NULL)
    {
        ofree((node->value).string_value);
    }
    if (node->key != NULL)
    {
        ofree(node->key);
    }
    if (node != NULL)
    {
        ofree(node);
    }
    return;
}

/*
    unsigned short       current_level;
    OlyDSNodeValueType   vt;
    OlyDSNodeValueType   parent_vt;
    long                 tuple;
    OChar               *key;
    OlyDSValue           value;
    OlyDSNode           *parent_node;
    */

OlyStatus  descend_one_level( OlyDSNode **node )
{
    OlyStatus  status = OLY_OKAY;
    OlyDSNode *next_node = NULL;
    if (((*node)->current_level + 1) > MAX_NODE_DEPTH)
    {
        status = OLY_ERR_NODES_TOO_DEEP;
        return status;
    }
    /* TODO: work out buffering! */
    /*    if ((((*node)->vt) == OLY_NODE_VALUE_TYPE_MAP) 
            && )
    {
        if (((*node)->ochar_buffer + 
                ( u_strlen((*node)->ochar_buffer) + 2 ))< ((*node)->ochar_buffer_end))
        next_node->ochar_buffer = ( (*node)->ochar_buffer 
                + ( u_strlen((*node)->ochar_buffer) + 2 ) );
    OChar               *ochar_buffer;
    OChar               *ochar_buffer_end;
    char                *char_buffer;
    char                *char_buffer_end;
    }
    */
    next_node = new_oly_ds_node( &status );
    next_node->parent_node = *node;
    next_node->parent_vt = (*node)->vt;
    next_node->current_level = ((*node)->current_level + 1);
    *node = next_node;
    return status;
}

OlyStatus  ascend_one_level( OlyDSNode **node )
{
    OlyStatus  status = OLY_OKAY;
    OlyDSNode *next_node = NULL;
    if (((*node)->current_level - 1) < 0)
    {
        status = OLY_ERR_NODES_TOO_SHALLOW;
        return status;
    }
    next_node = (*node)->parent_node ;

    return status;
}

OlyStatus  set_node_key(OlyDSNode *node, OChar *key );
OChar     *get_node_key(OlyDSNode *node, OlyStatus *status);

OlyStatus  set_node_string_value(OlyDSNode *node, char *value);
OChar     *get_node_string_value(OlyDSNode *node, OlyStatus *status);

OlyStatus  set_node_float_value(OlyDSNode *node, const double value);
double     get_node_float_value(OlyDSNode *node, OlyStatus *status);
OlyStatus  set_node_int_value(OlyDSNode *node, const long value);
long       get_node_int_value(OlyDSNode *node, OlyStatus *status);

OlyStatus  set_node_value_type(OlyDSNode *node, OlyDSNodeValueType *value_type);
OlyDSNodeValueType get_node_value_type(OlyDSNode *node, OlyStatus *status);
OlyDSNodeValueType get_node_parent_type(OlyDSNode *node, OlyStatus *status);


