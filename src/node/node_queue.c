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
#include "node/pvt_node.h"

OlyStatus 
open_node_queue(OlyNodeQueue **strbuf)
{
    OlyStatus status = OLY_OKAY;
    return status;
}

void
close_node_queue(OlyNodeQueue *strbuf)
{
    return;
}

/* copies at most *length OChars into the string buffer. */
OlyStatus
enqueue_to_node_queue( OlyNodeQueue *dest, const OChar *source,
        const size_t length )
{
    OlyStatus status = OLY_OKAY;
    return status;
};

/* copies at most *length OChars into the destination. *length holds the number of OChars actually provided at the end. dequeue removes them */
OlyStatus dequeue_from_node_queue(OlyNodeQueue *source, OChar **dest, size_t *length )
{
    OlyStatus status = OLY_OKAY;
    return status;
}

