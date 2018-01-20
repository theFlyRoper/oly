/* oly_run.c - Main process. Coordinates threads, data, etc. License GPL 2+{{{
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
#include "oly/data_source.h"
#include "oly/core.h"
#include "pvt_core.h"
#include "oly/node.h"
#include "oly/string_buffer.h"

static OlyStatus oly_receive( OlyDataSource *ds );
static OlyStatus oly_send( OlyDataSource *ds );

OlyStatus oly_run( void )
{
    OlyStatus status = OLY_OKAY;
    OlyDataSource   *in_ds, *out_ds;
    in_ds = oly->inbound;
    out_ds = oly->outbound;

    while ((status != OLY_ERR_NODE_QUEUE_FULL) && (status != OLY_ERR_BUFFER_OVERFLOW))
    {
        /* the dispatch function calls enqueue_ds_node when a single node is ready,
         * then passes OLY_WARN_NODE_PRODUCED as a status. */
        status = run_ds_dispatch_function( in_ds );
        if (status == OLY_WARN_NODE_PRODUCED)
        {
            status = OLY_OKAY;
        }
        HANDLE_STATUS_AND_RETURN(status);
        status = oly_receive(in_ds);
        if (status == OLY_ERR_NODES_TOO_SHALLOW)
        {
            printf("Shallow. Breaking.\n");
            break;
        }
        HANDLE_STATUS_AND_RETURN(status);
    }
    
    while (status != OLY_ERR_NODE_QUEUE_EMPTY)
    {
        status = oly_send(out_ds);
        if (status == OLY_ERR_NODE_QUEUE_EMPTY)
        {
            printf("empty, complete.\n");
            status = OLY_OKAY;
            break;
        }

        status = run_ds_dispatch_function( out_ds );
    }
    
    return status;
};

static OlyStatus oly_receive( OlyDataSource *ds )
{
    OlyStatus status = OLY_OKAY;
    OlyNode *node;

    status = collect_nodes( ds, &node);
    HANDLE_STATUS_AND_RETURN(status);
    status = enqueue_to_node_queue( oly->node_queue , node );
    HANDLE_STATUS_AND_RETURN(status);
    print_node(node);
    return status;
}

static OlyStatus oly_send( OlyDataSource *ds )
{
    OlyStatus status = OLY_OKAY;
    OlyNode *out_node;

    status = dequeue_from_node_queue( oly->node_queue , &out_node );
    HANDLE_STATUS_AND_RETURN(status);
    status = dispense_nodes( ds, out_node);
    HANDLE_STATUS_AND_RETURN(status);
    return status;
}

