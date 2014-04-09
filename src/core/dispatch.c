/* dispatch.c - main Oly object data source, node etc. functions. License GPL 2+{{{
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
#include "oly/core.h"
#include "pvt_core.h"

OlyStatus set_inbound_data_source( OlyDataSource *ds )
{
    oly->status = get_data_source_status(ds);
    HANDLE_STATUS_AND_RETURN(oly->status);
    oly->status = set_data_source_direction( ds, OLY_DS_IN );
    oly->inbound = ds;
    return oly->status;
}

OlyStatus set_outbound_data_source( OlyDataSource *ds )
{
    oly->status = get_data_source_status(ds);
    HANDLE_STATUS_AND_RETURN(oly->status);
    oly->status = set_data_source_direction( ds, OLY_DS_OUT );
    oly->outbound = ds;
    return oly->status;
}

OlyStatus get_inbound_data_source( OlyDataSource **ds )
{
    *ds = oly->inbound;
    return OLY_OKAY;
}

OlyStatus get_outbound_data_source( OlyDataSource **ds )
{
    *ds = oly->outbound;
    return OLY_OKAY;
}

OlyStatus enqueue_node_list(OlyNode **node_list, size_t node_count)
{
    OlyStatus status = OLY_OKAY;
    return status;
}

OlyStatus dequeue_node_list( OlyNode **node_list, size_t capacity, size_t node_count )
{
    OlyStatus status = OLY_OKAY;
    return status;
}

