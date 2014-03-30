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
    new_queue = (OlyNodeQueue *)omalloc(sizeof(OlyNodeQueue));
    new_queue->queue_size = get_node_queue_max();
    new_queue->stack = NULL;
    new_queue->queue_start = (OlyNode *)omalloc(new_queue->queue_size / sizeof(OlyNode));
    new_queue->queue_end = (new_queue->queue_start + (new_queue->queue_size / sizeof(OlyNode)));
    new_queue->in = new_queue->queue_start;
    new_queue->out = new_queue->queue_start;
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
    free(q);
    return;
}

/*  */
OlyStatus
enqueue_to_node_queue( OlyNodeQueue *q, OlyNode *n )
{
    OlyStatus status = OLY_OKAY;
    if (((q->out <= q->in) && (q->in < q->queue_end)) 
        || ((q->queue_start <= q->in) && (q->in < q->out) && (q->in+1 != q->out)))
    {
        q->in++;
        copy_node(n, q->in);
    }
    else if ((q->in == q->queue_end) && (q->out != q->queue_start))
    {
        q->in = q->queue_start;
        copy_node(n, q->in);
    }
    else
    {
        status = OLY_ERR_NODE_QUEUE_FULL;
    }
    return status;
};

/* */
OlyStatus dequeue_from_node_queue(OlyNodeQueue *q, OlyNode **node_out)
{
    OlyStatus status = OLY_OKAY;
    if (((q->queue_start <= q->out) && (q->out < q->in))
        || ((q->in < q->out) &&(q->out < q->queue_end)))
    {
        q->out++;
        copy_node(q->out, (*node_out));
    }
    else if (q->out == q->queue_end)
    {
        q->out = q->queue_start;
        copy_node(q->out, (*node_out));
    }
    else
    {
        status = OLY_ERR_NODE_QUEUE_EMPTY;
    }
    return status;
};

