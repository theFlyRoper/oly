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

static void
print_node_value(OlyNodeValue nv, OlyTagType type);

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
    new_node_local->depth           = 0;
    new_node_local->value.type      = OLY_TAG_TYPE_UNSET;
    new_node_local->node_id         = 0;
    new_node_local->key             = NULL;
    new_node_local->parent_node     = NULL;
    new_node_local->collection_end  = false;
    new_node_local->has_key         = false;
    
    new_node_local->value.value.string_value = NULL;
    (*new_node) = new_node_local;
    return status;
}

void
print_node( OlyNode *n )
{
    if (n->key != NULL)
    {
        u_fprintf(u_stdout, "Key: ");
        u_fprintf_u(u_stdout, n->key);
        u_fprintf(u_stdout," ");
    }
    if (n->parent_node != NULL)
    {
        u_fprintf(u_stdout, "Parent %lli, ", n->parent_node->node_id);
    }

    u_fprintf(u_stdout, "Tuple: %lli, Depth: %u, ", n->node_id, n->depth);
    print_node_value(n->value, n->value.type);
}

static void
print_node_value(OlyNodeValue nv, OlyTagType type)
{
    switch ( type )
    {
        case OLY_TAG_SCALAR_STRING:
            u_fprintf(u_stdout, "STRING: \"%.40S\"\n", nv.value.string_value);
            break;
        case OLY_TAG_SCALAR_FLOAT:
            printf("FLOAT: (%f)\n", nv.value.float_value);
            break;
        case OLY_TAG_SCALAR_INT:
            printf("INT: (%li)\n", nv.value.int_value);
            break;
        case OLY_TAG_COMPLEX_MAP:
            printf("MAP\n");
            break;
        case OLY_TAG_COMPLEX_SEQUENCE:
            printf("SEQUENCE\n");
            break;
        default:
            printf("UNKNOWN\n");
            break;
    }

}


OlyStatus
new_node_value( OlyNodeValue **new_node)
{
    OlyStatus status = OLY_OKAY;
    (*new_node) = (OlyNodeValue *)ocalloc(1,sizeof(OlyNodeValue));
    if ((*new_node) == NULL)
    {
        status = OLY_ERR_NOMEM;
    }
    return status;
}

OlyStatus
reset_node( OlyNode *node )
{
    node->value.type                = OLY_TAG_TYPE_UNSET;
    node->node_id             = 0;
    node->key               = NULL;
    node->parent_node       = NULL;
    node->value.value.string_value = NULL;
    node->collection_end    = false;
    return OLY_OKAY;
}

void 
close_oly_ds_node( OlyNode *node )
{
    if (node->collection_end == true)
    {
        close_oly_ds_node(node->parent_node);
    }
    /* string values handled in stringbuffer */
    /*
    if (((node->value).string_value) != NULL)
    {
        free((node->value).string_value);
    }
    if (node->key != NULL)
    {
        free(node->key);
    }
    */
    if (node != NULL)
    {
        free(node);
    }
    return;
}

OlyStatus
get_node_string_value(OlyNode *node, OChar **value_out)
{
    OlyStatus status = OLY_OKAY;
    (*value_out) = node->value.value.string_value ;
    return status;
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
}

/* string values must point to a buffer, so they are set separately. */
OlyStatus 
set_node_string_value(OlyNode *output, const OChar *value)
{
    OlyStatus status = OLY_OKAY;
    output->value.value.string_value = (OChar *)value;
    return status;
}

/* sets the key value*/
OlyStatus 
set_node_key(OlyNode *output, const OChar *key_in)
{
    OlyStatus status = OLY_OKAY;
    output->key = (OChar *)key_in;
    return status;
}

/* sets the value if node type is float or int and checks for errors. */
OlyStatus 
set_node_value(OlyNode *node, void *value, OlyTagType type)
{
    OlyStatus status = OLY_OKAY;
    OlyNodeValue *output = &(node->value);

    if ((type < 0) || (type > OLY_TAG_MAX))
    {
        status = OLY_ERR_ILLEGAL_TAG ;
    }
    node->value.type = type;
    if ( value != NULL )
    {
        switch ( type )
        {
            case OLY_TAG_SCALAR_STRING:
                break;
            case OLY_TAG_SCALAR_FLOAT:
                (*output).value.float_value = *((double *)value);
                break;
            case OLY_TAG_SCALAR_INT:
                (*output).value.int_value = *((long *)value);
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
            case OLY_TAG_COMPLEX_MAP:
            case OLY_TAG_COMPLEX_SEQUENCE:
                break;
            default:
                status = OLY_ERR_NODE_MUST_HAVE_VALUE;
                break;
        }
    }
    return status;
}

OlyStatus 
set_node_node_id(OlyNode *node, int64_t node_id)
{
    node->node_id = node_id;
    return OLY_OKAY;
}

OlyStatus
get_node_node_id(OlyNode *node, int64_t *node_id_out )
{
    OlyStatus status = OLY_OKAY;
    (*node_id_out) = node->node_id ;
    return status;
}

OlyStatus 
set_node_has_key(OlyNode *node)
{
    node->has_key = true;
    return OLY_OKAY;
}

OlyStatus 
unset_node_has_key(OlyNode *node)
{
    node->has_key = false;
    return OLY_OKAY;
}

OlyStatus
node_has_key(OlyNode *node)
{
    OlyStatus status = OLY_OKAY;
    if (node->has_key == false)
    {
        status = OLY_WARN_NODE_HAS_NO_KEY ;
    }
    return status;
}

OlyStatus
get_node_parent( const OlyNode *node, OlyNode **parent)
{
    OlyStatus status = OLY_OKAY;
    if (node->depth < 1)
    {
        status = OLY_ERR_NODES_TOO_SHALLOW;
        (*parent) = NULL;
    }
    (*parent) = node->parent_node;
    return status;
}

OlyStatus 
copy_node(const OlyNode *source, OlyNode *dest)
{
    dest->depth             = source->depth;
    dest->node_id           = source->node_id;
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

OlyStatus  
descend_one_level( OlyNode *node )
{
    OlyStatus  status = OLY_OKAY;
    if ((node->depth + 1) > MAX_NODE_DEPTH)
    {
        status = OLY_ERR_NODES_TOO_DEEP;
        return status;
    }
    (node->depth)++;
    return status;
}

OlyStatus  
ascend_one_level( OlyNode *node )
{
    OlyStatus  status = OLY_OKAY;
    if ((node->depth - 1) < 0)
    {
        status = OLY_ERR_NODES_TOO_SHALLOW;
        return status;
    }
    (node->depth)--;
    return status;
}

