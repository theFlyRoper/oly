/* string_buffer.c - Oly string buffer functions License GPL 2+{{{
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
#include "oly/config.h"
#include "oly/olytypes.h"
#include <assert.h>
#include "oly/string_buffer.h"
#include "core/pvt_string_buffer.h"

static OlyStatus is_buffer_empty(OlyStringBuffer *strbuf);
static OlyStatus is_buffer_full(OlyStringBuffer *strbuf, size_t requested);

static void flip_write(OlyStringBuffer *strbuf);

void 
flip_write(OlyStringBuffer *strbuf)
{
    if (strbuf->write_to_a == true)
    {
        strbuf->write_to_a = false;
    }
    else
    {
        strbuf->write_to_a = true;
    }
}

static OlyStatus length_at_end( OlyStringBuffer *strbuf, size_t *length );

OlyStatus length_at_end( OlyStringBuffer *strbuf, size_t *length )
{
    *length = (strbuf->buffer_size 
                - ((( strbuf->end_a >= strbuf->end_b ) ? strbuf->end_a : strbuf->end_b)
                    - strbuf->buffer));
    return OLY_OKAY;
}

static OlyStatus length_at_start( OlyStringBuffer *strbuf, size_t *length );

OlyStatus length_at_start( OlyStringBuffer *strbuf, size_t *length )
{
    OChar *top_start = (( strbuf->begin_a >= strbuf->begin_b ) ? strbuf->begin_a : strbuf->begin_b );
    if (top_start > strbuf->buffer)
    {
    *length = ( strbuf->buffer_size 
                - (top_start
                - (( strbuf->end_a <= strbuf->end_b ) ? strbuf->end_a : strbuf->end_b 
                )));
    }
    else
    {
        length_at_end( strbuf, length );
    }
    return OLY_OKAY;
}

OlyStatus 
is_buffer_empty(OlyStringBuffer *strbuf)
{
    OlyStatus status = OLY_OKAY;
    if ((strbuf->begin_a == NULL) && (strbuf->begin_b == NULL))
    {
        status = OLY_WARN_BUFFER_EMPTY ;
    }
    return status;
}

OlyStatus 
is_buffer_full(OlyStringBuffer *strbuf, size_t requested)
{
    OlyStatus status = OLY_OKAY;
    size_t len_end = 0, len_start = 0;
    length_at_end(strbuf, &len_end);
    length_at_start(strbuf, &len_start);
    if ((len_end < requested) && (len_start < requested))
    {
        status = OLY_ERR_BUFFER_OVERFLOW;
    }
    return status;
}

OlyStatus 
open_string_buffer(OlyStringBuffer **strbuf)
{
    OlyStatus status = OLY_OKAY;
    size_t max_len = get_main_string_buffer_max() ;
    OlyStringBuffer *new_buffer = ocalloc(1,sizeof(OlyStringBuffer));
    if (new_buffer == NULL)
    {
        status = OLY_ERR_NOMEM;
        HANDLE_STATUS_AND_DIE(status);
    }
    new_buffer->buffer = (omalloc( max_len * sizeof(OChar))); 
    if (new_buffer->buffer == NULL)
    {
        close_string_buffer(new_buffer);
        status = OLY_ERR_NOMEM;
        HANDLE_STATUS_AND_DIE(status);
    }
    new_buffer->begin_a     = NULL;
    new_buffer->end_a       = NULL;
    new_buffer->begin_b     = NULL;
    new_buffer->end_b       = NULL;
    new_buffer->buffer_size = max_len;
    new_buffer->write_to_a = true;
    *(new_buffer->buffer + (max_len - 1)) = (OChar)0;
    (*strbuf)               = new_buffer;
    return status;
}

void
close_string_buffer(OlyStringBuffer *strbuf)
{
    if (strbuf->buffer != NULL)
    {
        strbuf->begin_a     = NULL;
        strbuf->end_a       = NULL;
        strbuf->begin_b     = NULL;
        strbuf->end_b       = NULL;
        free(strbuf->buffer);
    }
    if (strbuf != NULL)
    {
        free(strbuf);
    }
}

/* copies at most *length OChars into the string buffer. */
OlyStatus
enqueue_to_string_buffer( OlyStringBuffer *dest, const OChar *source,
        const size_t length )
{

    OlyStatus status = OLY_OKAY;
    size_t len_end = 0, len_start = 0, available = 0;
    length_at_start(dest, &len_start);
    length_at_end(dest, &len_end);
    if (len_start > len_end )
    {
        available = len_start;
        flip_write(dest);
    }
    else
    {
        available = len_end;
    }
    status = is_buffer_full(dest, length);
    HANDLE_STATUS_AND_RETURN(status);
    
    if (( true == dest->write_to_a ) 
            && ((available - (dest->end_a - dest->buffer)) > length))
    {
        u_strncpy( dest->end_a, source, (length+1) ) ;
        dest->end_a += (length+2) ;
    }
    else if (( false == dest->write_to_a ) 
            && ((available - (dest->end_b - dest->buffer)) > length))
    {
        u_strncpy( dest->end_b, source, (length+1) ) ;
        dest->end_b += (length+2) ;
    }
    else 
    {
        status = OLY_ERR_BUFFER_OVERFLOW;
    }
    return status;
};

/* copies at most *length OChars into the destination. *length holds the number of OChars actually provided at the end. dequeue removes them */
OlyStatus dequeue_from_string_buffer(OlyStringBuffer *strbuf, OChar **dest, size_t *length )
{
    OlyStatus status = OLY_OKAY;
    OChar *read = ((strbuf->begin_a >= strbuf->begin_b) ? strbuf->begin_a : strbuf->begin_b);
    status = is_buffer_empty(strbuf);
    HANDLE_STATUS_AND_RETURN(status);

    u_strncpy( (*dest), read, (*length+1) ) ;
    (*length) = u_strlen(*dest);
    return status;
}

