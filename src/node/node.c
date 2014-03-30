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

#include "oly/common.h"
#include "oly/olytypes.h"
#include "node/pvt_node.h"

OlyStatus 
new_oly_node( OlyNode **new_node )
{
    OlyStatus       status = OLY_OKAY;
    OlyNode         *new_node_local = NULL;
    HANDLE_STATUS_AND_RETURN(status);
    new_node_local = omalloc(sizeof(OlyNode));
    if ( new_node_local == NULL )
    {
        status = OLY_ERR_NOMEM;
        HANDLE_STATUS_AND_RETURN(status);
    }
    new_node_local->depth             = 0;
    new_node_local->vt                = OLY_NODE_VALUE_TYPE_UNSET;
    new_node_local->tuple             = 0;
    new_node_local->key               = NULL;
    new_node_local->parent_node       = NULL;
    
    (new_node_local->value).string_value = NULL;
    (*new_node) = new_node_local;
    return status;
}

OlyStatus
new_node_value( OlyNodeValue **new_node_value)
{
    OlyStatus status = OLY_OKAY;
    (*new_node_value) = (OlyNodeValue *)ocalloc(1,sizeof(OlyNodeValue));
    if ((*new_node_value) == NULL)
    {
        status = OLY_ERR_NOMEM;
    }
    return status;
}

OlyStatus
reset_node( OlyNode *node )
{
    node->depth             = 0;
    node->vt                = OLY_NODE_VALUE_TYPE_UNSET;
    node->tuple             = 0;
    node->key               = NULL;
    node->parent_node       = NULL;
    (node->value).string_value = NULL;
    return OLY_OKAY;
}

void 
close_oly_ds_node( OlyNode *node )
{
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
get_node_key(OlyNode *node, OChar **key_out)
{
    OlyStatus status = node_has_key(node);
    if ( OLY_WARN_NODE_HAS_NO_KEY == status )
    {
        key_out = NULL;
    }
    else
    {
        (*key_out) = node->key;
    }
    return status;
};

/* string values must point to a buffer, so they are set separately. */
OlyStatus 
set_node_string_value(OlyNode *output, const OChar *value)
{
    OlyStatus status = OLY_OKAY;
    (output->value).string_value = (OChar *)value;
    return status;
}

/* sets the value if node type is float or int and checks for errors. */
OlyStatus 
set_node_value(OlyNode *node, void *value, OlyNodeValueType type)
{
    OlyStatus status = OLY_OKAY;
    OlyNodeValue *output = &(node->value);

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
                (*output).float_value = *((double *)value);
                break;
            case OLY_NODE_VALUE_SCALAR_INT:
                (*output).int_value = *((long *)value);
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

OlyStatus 
set_node_tuple(OlyNode *node, int64_t tuple)
{
    node->tuple = tuple;
    return OLY_OKAY;
};

OlyStatus
get_node_tuple(OlyNode *node, int64_t *tuple_out )
{
    OlyStatus status = OLY_OKAY;
    (*tuple_out) = node->tuple ;
    return status;
};

OlyStatus 
unset_node_has_key(OlyNode *node)
{
    node->has_key = false;
    return OLY_OKAY;
};

OlyStatus
node_has_key(OlyNode *node)
{
    OlyStatus status = OLY_OKAY;
    if (node->has_key == false)
    {
        status = OLY_WARN_NODE_HAS_NO_KEY ;
    }
    return status;
};

OlyStatus
get_node_parent( const OlyNode *node, OlyNode **parent)
{
    OlyStatus status = OLY_OKAY;
    if (node->depth < 1)
    {
        status = OLY_ERR_NODES_TOO_SHALLOW;
        (*parent) = NULL;
    }
    return status;
}

OlyStatus 
copy_node(const OlyNode *source, OlyNode *dest)
{
    dest->depth             = source->depth;
    dest->vt                = source->vt;
    dest->tuple             = source->tuple;
    dest->key               = source->key;
    dest->parent_node       = source->parent_node;
    dest->value             = source->value;
    return OLY_OKAY ;
}

OlyStatus 
push_node(OlyNode **stack, OlyNode *node )
{
    OlyStatus status = OLY_OKAY;
    OlyNode   *stack_copy;
    status = new_oly_node( &stack_copy );
    HANDLE_STATUS_AND_RETURN(status);
    (node->depth)++;
    if ((*stack) != NULL)
    {
        node->parent_node = (*stack);
    }
    status = copy_node( node, stack_copy );
    HANDLE_STATUS_AND_RETURN(status);
    (*stack) = stack_copy;
    return status;
}

OlyStatus 
pop_node( OlyNode **stack, OlyNode **node_out )
{
    OlyStatus status = OLY_OKAY;
    OlyNode *old_stack;
    old_stack = (*stack);
    if ((*stack)->parent_node != NULL)
    {
        (*stack) = (*stack)->parent_node;
    }
    else 
    {
        status = OLY_ERR_NODES_TOO_SHALLOW;
        return status;
    }

    if (node_out != NULL)
    {
        (*node_out) = old_stack;
    }
    else
    {
        close_oly_ds_node( old_stack );
    }

    return status;
}


