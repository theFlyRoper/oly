/* node_queue.c - Oly string buffer functions License GPL 2+{{{
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
 * }}} */

#include "oly/common.h"
#include "oly/olytypes.h"
#include <assert.h>
#include "oly/config.h"
#include "node/pvt_node.h"

OlyStatus 
open_node_queue(OlyNodeQueue **q)
{
    OlyStatus status = OLY_OKAY;
    OlyNodeQueue *new_queue;
    size_t           max_key_size = MAX_KEY_LENGTH;
    int64_t          queue_size = 0;
    new_queue = (OlyNodeQueue *)omalloc(sizeof(OlyNodeQueue));
    new_queue->key_max_length = max_key_size;
    get_main_config_int(OLY_CONFIG_NODE_QUEUE_MAX, &queue_size);
    new_queue->queue_size = (size_t)queue_size;
    new_queue->stack = NULL;
    new_queue->queue_start = (OlyNode *)omalloc(new_queue->queue_size / sizeof(OlyNode));
    new_queue->queue_end = (new_queue->queue_start + (new_queue->queue_size / sizeof(OlyNode)));
    new_queue->in = new_queue->queue_start;
    new_queue->out = new_queue->queue_start;
    status         = open_string_buffer(&(new_queue->string_buffer));
    *q = new_queue;
    return status;
}

void
close_node_queue(OlyNodeQueue *q)
{
    OlyNode *n;
    while (q->stack != NULL)
    {
        n = q->stack;
        q->stack = q->stack->parent_node;
        close_oly_ds_node(n);
        n = NULL;
    }
    free(q->queue_start);
    q->in = NULL;
    q->out = NULL;
    close_string_buffer(q->string_buffer);
    free(q);
    return;
}

OlyStatus
enqueue_to_node_queue( OlyNodeQueue *q, OlyNode *n )
{
    OlyStatus status = OLY_OKAY;
    OlyNode   *s, *prev;
    OChar     *result;
    size_t      key_len = 0, depth = 0;
    prev = (q->in);
    /* step in ptr fwd if not at end, or back to beginning if at end. Keep behind queue_end, else ERR */
    if (((q->out <= q->in) && (q->in < q->queue_end)) 
        || ((q->queue_start <= q->in) && (q->in < q->out) && (q->in+1 != q->out)))
    {
        q->in++;
    }
    else if ((q->in == q->queue_end) && (q->out != q->queue_start))
    {
        q->in = q->queue_start;
    }
    else
    {
        status = OLY_ERR_NODE_QUEUE_FULL;
    }
    HANDLE_STATUS_AND_RETURN(status);
    copy_node(n, q->in);

    if (q->in->depth > prev->depth)
    {
        status = push_node(&(q->stack), (q->in));
        HANDLE_STATUS_AND_RETURN(status);
    }
    
    depth = prev->depth;
    while (q->in->depth < depth)
    {
        prev->collection_end = true;
        status = pop_node(&(q->stack), &s );
        HANDLE_STATUS_AND_RETURN(status);
        depth--;
    }
    if ((q->in->value.type == OLY_TAG_COMPLEX_SEQUENCE ) 
            || (q->in->value.type == OLY_TAG_COMPLEX_MAP ))
    {
        q->in->parent_node = q->stack->parent_node ;
    }
    else
    {
        q->in->parent_node = q->stack ;
    }
    
    if  (q->in->key != NULL)
    {
        key_len = u_strlen(q->in->key);
        status = reserve_string_buffer( q->string_buffer, (key_len+2) );
        HANDLE_STATUS_AND_RETURN(status);
        status = enqueue_to_string_buffer(q->string_buffer, (q->in->key), &result, &key_len);
        q->in->key = result;
        HANDLE_STATUS_AND_RETURN(status);
    }

    if (OLY_TAG_SCALAR_STRING == q->in->value.type )
    {
        key_len = u_strlen(q->in->value.value.string_value);
        status = reserve_string_buffer( q->string_buffer, key_len );
        HANDLE_STATUS_AND_RETURN(status);
        status = enqueue_to_string_buffer(q->string_buffer, q->in->value.value.string_value, &result, &key_len);
        HANDLE_STATUS_AND_RETURN(status);
        q->in->value.value.string_value = result;
    }
    return status;
};

/* */
OlyStatus dequeue_from_node_queue(OlyNodeQueue *q, OlyNode **node_out)
{
    OlyNode   *prev;
    OlyStatus status = OLY_OKAY;
    prev = (q->out);
    if (((q->queue_start <= q->out) && (q->out < q->in))
        || ((q->in < q->out) && (q->out < q->queue_end)))
    {
        q->out++;
    }
    else if (q->out == q->queue_end)
    {
        q->out = q->queue_start;
    }
    else
    {
        status = OLY_ERR_NODE_QUEUE_EMPTY;
    }
    copy_node(q->out, (*node_out));
    reset_node(prev);
    return status;
};

