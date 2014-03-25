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
    
    new_node->depth             = 0;
    new_node->vt                = OLY_NODE_VALUE_TYPE_UNSET;
    new_node->tuple             = 0;
    new_node->key               = NULL;
    new_node->parent_node       = NULL;
    
    (new_node->value).string_value = NULL;
    
    return new_node;
}

OlyNodeValue *
new_node_value( void )
{
    return ocalloc(1,sizeof(OlyNodeValue));
}

void
reset_node( OlyNode *node )
{
    node->depth             = 0;
    node->vt                = OLY_NODE_VALUE_TYPE_UNSET;
    node->tuple             = 0;
    node->key               = NULL;
    node->parent_node       = NULL;
    (node->value).string_value = NULL;
}

void 
close_oly_ds_node( OlyNode *node )
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

OlyStatus  
descend_one_level( OlyNode **node )
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

OlyStatus  
ascend_one_level( OlyNode **node )
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

OChar *get_node_key(OlyNode *node, OlyStatus *status)
{
    if (*status != OLY_OKAY)
    {
        return NULL;
    }
    else
    {
        return node->key;
    }

};

/* string values must point to a buffer, so they are set separately. */
OlyStatus set_node_string_value(OlyNode *output, OChar *value)
{
    OlyStatus status = OLY_OKAY;
    (output->value).string_value = value;
    return status;
}

/* sets the value if node type is float or int and checks for errors. */
OlyStatus set_node_value(OlyNode *node, void *value, OlyNodeValueType type)
{
    OlyStatus status = OLY_OKAY;
    OlyNodeValue *output = node->value;

    if ((type <= OLY_NODE_VALUE_MIN) || (type > OLY_NODE_VALUE_MAX))
    {
        status = OLY_ERR_ILLEGAL_NODE_TYPE ;
    }
    node->vt = type;
    if ( value != NULL )
    {
        switch ( type )
        {
            case OLY_NODE_VALUE_SCALAR_STRING:
                break;
            case OLY_NODE_VALUE_SCALAR_FLOAT:
                (*output).float_value = (double)*value;
                break;
            case OLY_NODE_VALUE_SCALAR_INT:
                (*output).int_value = (long)*value;
                break;
            default:
                status = OLY_ERR_NODE_MUST_NOT_HAVE_VALUE;
                break;
        }
    }
    else
    {
        switch ( type )
        {
            case OLY_NODE_VALUE_TYPE_MAP:
            case OLY_NODE_VALUE_TYPE_SEQUENCE:
                break;
            default:
                status = OLY_ERR_NODE_MUST_HAVE_VALUE;
                break;
        }
    }
    return status;
}

OlyStatus set_node_tuple(OlyNode *node, int64_t tuple)
{
    node->tuple = tuple;
    return OLY_OKAY;
};

int64_t get_node_tuple(OlyNode *node, OlyStatus *status)
{
    *status = OLY_OKAY;
    return node->tuple ;
};

OlyStatus unset_node_has_key(OlyNode *node)
{
    node->has_key = 0x0;
};

unsigned char node_has_key(OlyNode *node)
{
    return node->has_key;
};

int64_t get_parent_tuple(OlyNode *node, OlyStatus *status)
{
    if (*status != OLY_OKAY)
    {
        return -1;
    }
    if (node->parent_node != NULL)
    {
        return node->parent_node->tuple;
    }
    else
    {
        return 0;
    }
}

OlyStatus copy_node(OlyNode *source, OlyNode *dest)
{
    dest->depth             = source->depth;
    dest->vt                = source->vt;
    dest->tuple             = source->tuple;
    dest->key               = source->key;
    dest->parent_node       = source->parent_node;
    dest->value             = source->value;
    return OLY_OKAY ;
}



