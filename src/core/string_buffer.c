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

#define STRBUFLOG printf("%s:%d:%s():\n\tbuf2ba: %zu, buf2ea %zu, buf2bb %zu, buf2eb %zu buf to end: %zu\n" \
        "", __FILE__, __LINE__, __func__, (strbuf->read_a-strbuf->buffer_start), \
        (strbuf->write_a-strbuf->buffer_start), (strbuf->read_b-strbuf->buffer_start), \
        (strbuf->write_b-strbuf->buffer_start), (strbuf->buffer_end-strbuf->buffer_start));

#define STRCOMMITLOG(str1, arg1, arg2) printf("%s:%d:%s():\n" \
        str1, __FILE__, __LINE__, __func__, arg1, arg2 );

/* we never actually need get_lower_start, since the lower start will always be the buffer start. */
static OlyStatus get_lower_end(OlyStringBuffer *strbuf, OChar **lower_end);
static OlyStatus get_higher_end(OlyStringBuffer *strbuf, OChar **higher_end);
static OlyStatus get_higher_start(OlyStringBuffer *strbuf, OChar **higher_start);
static OlyStatus length_write_space(OlyStringBuffer *strbuf, size_t *length_out);
static OlyStatus is_buffer_empty(OlyStringBuffer *strbuf);

static OlyStatus get_lower_end(OlyStringBuffer *strbuf, OChar **lower_end)
{
    OlyStatus status = OLY_OKAY;
    (*lower_end) = (( strbuf->write_a <= strbuf->write_b ) ? strbuf->write_a : strbuf->write_b );
    return status;
};

static OlyStatus get_higher_end(OlyStringBuffer *strbuf, OChar **higher_end)
{
    OlyStatus status = OLY_OKAY;
    (*higher_end) = (( strbuf->write_b >= strbuf->write_a ) ? strbuf->write_b : strbuf->write_a );
    return status;
};

static OlyStatus get_higher_start(OlyStringBuffer *strbuf, OChar **higher_start)
{
    OlyStatus status = OLY_OKAY;
    (*higher_start) = (( strbuf->read_b >= strbuf->read_a ) ? strbuf->read_b : strbuf->read_a );
    return status;
};


/* fills in length and changes the state for write if needed. */
static OlyStatus 
length_write_space(OlyStringBuffer *strbuf, size_t *length_out)
{
    OChar *read, *write;
    OlyStatus status = OLY_OKAY;
    size_t same_read_write = 0, diff_read_write = 0;
    status = get_higher_end(strbuf, &read);
    HANDLE_STATUS_AND_RETURN(status);
    write = (strbuf->buffer_end - 1);
    same_read_write = write - read;
    
    status = get_lower_end(strbuf, &read);
    HANDLE_STATUS_AND_RETURN(status);
    status = get_higher_start(strbuf, &write);
    HANDLE_STATUS_AND_RETURN(status);
    diff_read_write = write - read;

    /* if read and write are same buffer, then we do the state flip check here. */
    if (same_read_write >= diff_read_write)
    {
        *length_out = same_read_write;
    }
    else
    {
        switch(strbuf->state)
        {
            case WRITE_A_READ_A:
                strbuf->state = WRITE_B_READ_A;
                break;
            case WRITE_B_READ_B:
                strbuf->state = WRITE_A_READ_B;
                break;
            default:
                break;
        }
        *length_out = diff_read_write;
    }
    return status;
}

static OlyStatus 
is_buffer_empty(OlyStringBuffer *strbuf)
{
    OlyStatus status = OLY_OKAY;
    if ((strbuf->read_a == strbuf->buffer_start) && (strbuf->read_b == strbuf->buffer_start))
    {
        status = OLY_WARN_BUFFER_EMPTY ;
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
    OLYLOG(" max_len: %zu\n", max_len);
    new_buffer->buffer_start = (omalloc( max_len * sizeof(OChar))); 
    if (new_buffer->buffer_start == NULL)
    {
        close_string_buffer(new_buffer);
        status = OLY_ERR_NOMEM;
        HANDLE_STATUS_AND_DIE(status);
    }
    new_buffer->read_a      = new_buffer->buffer_start;
    new_buffer->write_a     = new_buffer->buffer_start;
    new_buffer->read_b      = new_buffer->buffer_start;
    new_buffer->write_b     = new_buffer->buffer_start;
    new_buffer->buffer_end  = (new_buffer->buffer_start + (max_len - 1));
    new_buffer->state       = WRITE_A_READ_A;
    new_buffer->reserve_start = NULL;
    new_buffer->reserve_end = NULL;
    *(new_buffer->buffer_end) = (OChar)0;
    (*strbuf)               = new_buffer;
    return status;
}

void
close_string_buffer(OlyStringBuffer *strbuf)
{
    if (strbuf->buffer_start != NULL)
    {
        strbuf->read_a     = NULL;
        strbuf->write_a       = NULL;
        strbuf->read_b     = NULL;
        strbuf->write_b       = NULL;
        free(strbuf->buffer_start);
    }
    if (strbuf != NULL)
    {
        free(strbuf);
    }
}

/* locks strbuf for writing by setting reserve ptr */
OlyStatus
reserve_string_buffer( OlyStringBuffer *strbuf, const size_t length )
{
    OlyStatus status = OLY_OKAY;
    size_t available = 0;
    assert((strbuf->buffer_start <= strbuf->write_a) 
                && (strbuf->write_a < strbuf->buffer_end));
    assert((strbuf->buffer_start <= strbuf->write_b) 
                && (strbuf->write_b < strbuf->buffer_end));
    if (strbuf->reserve_start != NULL)
    {
        status = OLY_WARN_BUFFER_WRITE_LOCK;
        return status;
    }
    status = length_write_space(strbuf, &available);
    HANDLE_STATUS_AND_RETURN(status);
    if ( available < (length+1) )
    {
        status = OLY_ERR_BUFFER_OVERFLOW;
        return status;
    }

    if ((strbuf->state == WRITE_A_READ_A) || (strbuf->state == WRITE_A_READ_B))
    {
        strbuf->reserve_start = strbuf->write_a;
    }
    else
    {
        strbuf->reserve_start = strbuf->write_b;
    }
    strbuf->reserve_end = (strbuf->reserve_start + (length+1));
    return status;
};

/* adds the OChar string to the buffer.  If the input string is the same 
 * length as allocated space, it returns OLY_WARN_LONG_STRING and leaves the write pointer alone, otherwise it moves the write pointer past the 
 * string terminator (that is, '\0'). */
OlyStatus
enqueue_to_string_buffer( OlyStringBuffer *strbuf, const OChar *string, size_t *length_out )
{
    OlyStatus status = OLY_OKAY;
    size_t available = 0, jump_spaces = 2;

    assert((strbuf->reserve_start != NULL) && (strbuf->reserve_end != NULL));
/* Problem here.  If the string fills the buffer, it continues.  If it continues,
 * we need to let the user know. We currently pass back a warning and move
 * the item exactly that amount, preserving the string in its entirety.  */
    available = (strbuf->reserve_end-strbuf->reserve_start-1);
    u_strncpy(strbuf->reserve_start, string, available);
    *(strbuf->reserve_end) = (OChar)0;
    *length_out = u_strlen(strbuf->reserve_start);
    STRBUFLOG;
    STRCOMMITLOG("Available: %zu, *length_out: %zu\n", available, *length_out);
    if ( available < *length_out )
    {
        status = OLY_ERR_BUFFER_OVERFLOW;
        return status;
    }
    else if ( available == *length_out )
    {
        status = OLY_WARN_LONG_STRING;
        if (true == strbuf->keep_long_strings)
        {
            jump_spaces = 1;
        }
        else
        {
            return status;
        }
    }
    
    if ((strbuf->state == WRITE_A_READ_A) || (strbuf->state == WRITE_A_READ_B))
    {
        strbuf->write_a += (*length_out + jump_spaces);
    }
    else
    {
        strbuf->write_b += (*length_out + jump_spaces);
    }
    STRCOMMITLOG("write_a-bufstart: %zu, write_b-bufstart: %zu\n", 
            strbuf->write_a - strbuf->buffer_start ,
            strbuf->write_b - strbuf->buffer_start );
    strbuf->reserve_start = NULL;
    strbuf->reserve_end = NULL;

    return status;
};

/* copies at most *length OChars into the destination. *length holds the number of OChars actually provided at the end. dequeue removes them */
OlyStatus 
dequeue_from_string_buffer(OlyStringBuffer *strbuf, OChar **dest, const size_t size_in,
        size_t *length )
{
    OlyStatus status = OLY_OKAY;
    OChar *read = ((strbuf->read_a >= strbuf->read_b) ? strbuf->read_a : strbuf->read_b);
    status = is_buffer_empty(strbuf);
    HANDLE_STATUS_AND_RETURN(status);
    STRBUFLOG

    u_strncpy( (*dest), read, size_in ) ;
    (*length) = u_strlen(*dest);

    if (read == strbuf->read_a)
    {
        if (strbuf->read_a+(*length) == strbuf->write_a)
        {
            strbuf->read_a = strbuf->buffer_start;
        }
        else
        {
            strbuf->read_a += (*length);
        }
    }
    else
    {
        if (strbuf->read_b+(*length) == strbuf->write_b)
        {
            strbuf->read_b = strbuf->buffer_start;
        }
        else
        {
            strbuf->read_b += (*length);
        }
    }

    return status;
}

