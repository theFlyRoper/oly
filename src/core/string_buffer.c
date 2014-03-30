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
#include <assert.h>

#include "oly/config.h"
#include "oly/olytypes.h"
#include "oly/string_buffer.h"
#include "core/pvt_string_buffer.h"

#include "oly/oly_dev.h"
#define STRCOMMITLOG(str1, arg1, arg2) printf("%s:%d:%s():\n" \
        str1, __FILE__, __LINE__, __func__, arg1, arg2 );

static OlyStatus get_lower_end(OlyStringBuffer *strbuf, OChar **lower_end);



/* we never actually need get_lower_start, since the lower start will always be the buffer start. */
static OlyStatus get_lower_end(OlyStringBuffer *strbuf, OChar **lower_end);
static OlyStatus get_higher_end(OlyStringBuffer *strbuf, OChar **higher_end);
static OlyStatus get_higher_start(OlyStringBuffer *strbuf, OChar **higher_start);
static OlyStatus is_buffer_empty(OlyStringBuffer *strbuf);
static OlyStatus release_reserve(OlyStringBuffer *strbuf);
static void length_at_end(OlyStringBuffer *strbuf, size_t *space_at_end);
static void length_at_start(OlyStringBuffer *strbuf, size_t *space_at_start);

static OlyStatus get_lower_end(OlyStringBuffer *strbuf, OChar **lower_end)
{
    OlyStatus status = OLY_OKAY;
    (*lower_end) = (( strbuf->write_a <= strbuf->write_b ) ? strbuf->write_a : strbuf->write_b );
    return status;
}

static OlyStatus get_higher_end(OlyStringBuffer *strbuf, OChar **higher_end)
{
    OlyStatus status = OLY_OKAY;
    (*higher_end) = (( strbuf->write_b >= strbuf->write_a ) ? strbuf->write_b : strbuf->write_a );
    return status;
}

static OlyStatus get_higher_start(OlyStringBuffer *strbuf, OChar **higher_start)
{
    OlyStatus status = OLY_OKAY;
    (*higher_start) = (( strbuf->read_b >= strbuf->read_a ) ? strbuf->read_b : strbuf->read_a );
    return status;
}

static void
length_at_end(OlyStringBuffer *strbuf, size_t *space_at_end)
{
    OChar *read, *write;
    get_higher_end(strbuf, &read);
    write = (strbuf->buffer_end - 1);
    *space_at_end = write - read;
}

static void
length_at_start(OlyStringBuffer *strbuf, size_t *space_at_start)
{
    OChar *read, *write;
    get_lower_end(strbuf, &read);
    get_higher_start(strbuf, &write);
    *space_at_start = write - read;
}

/* fills in length and changes the state for write if needed. */
OlyStatus 
space_available(OlyStringBuffer *strbuf, size_t *space_available)
{
    OlyStatus status = OLY_OKAY;
    size_t space_at_end = 0, space_at_start = 0;
    if ((strbuf->state == WRITE_A_READ_A) || (strbuf->state == WRITE_B_READ_B))
    {
        /* we collect two numbers.  space_at_end holds how much space at 
        * the end of the buffer, and space_at_start holds how much is at
        * the start. return the greater of the two. 
        * WRITE_A_READ_A and WRITE_B_READ_B are the states that
        * may transition in this function. */
        length_at_start(strbuf, &space_at_start);
        length_at_end(strbuf, &space_at_end);
        if ( space_at_end >= space_at_start ) 
        {
            *space_available = space_at_end;
        }
        else if (strbuf->state == WRITE_A_READ_A)
        {
            strbuf->state = WRITE_B_READ_A;
            *space_available = space_at_start;
        }
        else
        {
            strbuf->state = WRITE_A_READ_B;
            *space_available = space_at_start;
        }
#ifdef DEBUG_STRING_BUFFER
        report_values(strbuf, __func__);
#endif
    }
    else
    {
        length_at_start(strbuf, space_available);
    }
    return status;
}

static OlyStatus 
is_buffer_empty(OlyStringBuffer *strbuf)
{
    OlyStatus status = OLY_OKAY;
    if ((strbuf->write_a == strbuf->buffer_start) && (strbuf->write_b == strbuf->buffer_start))
    {
        status = OLY_WARN_BUFFER_EMPTY ;
    }
    return status;
}

static OlyStatus release_reserve(OlyStringBuffer *strbuf)
{
    OlyStatus status = OLY_OKAY;
    assert((strbuf->reserve_start != NULL) && (strbuf->reserve_end != NULL));
    strbuf->reserve_start = NULL;
    strbuf->reserve_end = NULL;
    return status;
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
    status = space_available(strbuf, &available);
    if ( status != OLY_OKAY )
    {
        return status;
    }
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
}

/* adds the OChar string to the buffer.  If the input string is the same 
 * length as allocated space, it returns OLY_WARN_LONG_STRING and leaves the write pointer alone, otherwise it moves the write pointer past the 
 * string terminator (that is, '\0'). */
OlyStatus
enqueue_to_string_buffer( OlyStringBuffer *strbuf, const OChar *string, size_t *length_out )
{
    OlyStatus status = OLY_OKAY;
    size_t available = 0;
    OChar  *copier = NULL;
    if((strbuf->reserve_start == NULL) || (strbuf->reserve_end == NULL))
    {
        status = OLY_ERR_NO_RESERVATION;
        return status;
    };
    
/* Problem here.  If the string fills the buffer, it continues.  If it continues,
 * we need to let the user know. We currently pass back a warning and 
 * check a variable called keep_long_strings.  If true, we move the
 * write pointer to exactly the end of the string.  If false, we ignore the long
 * string. */
    if (*string == 0)
    {
        release_reserve(strbuf);
        *length_out = 0;
        return status;
    }
    copier = strbuf->reserve_start;
    available = ((strbuf->reserve_end - strbuf->reserve_start) - 1);

    while ((*(copier++) = *(string++)) != 0 && ( copier < strbuf->reserve_end))
        ;
    *copier = 0;
    *length_out = (copier - strbuf->reserve_start);
    if ( available == *length_out )
    {
        status = OLY_WARN_LONG_STRING;
        if (false == strbuf->keep_long_strings)
        {
            return status;
        }
    }
    copier++;
    
    if ((strbuf->state == WRITE_A_READ_A) || (strbuf->state == WRITE_A_READ_B))
    {
        strbuf->write_a = copier;
    }
    else
    {
        strbuf->write_b = copier;
    }
    release_reserve(strbuf);
    return status;
}

/* copies at most *length OChars into the destination. *length holds the number of 
 * OChars actually provided at the end. dequeue removes them from the output
 * buffer if successful.  returns OLY_WARN_BUFFER_EMPTY if the buffer is empty,
 * and OLY_ERR_BUFFER_OVERFLOW if there was not enough space in the output buffer.
 * if there was not enough space, sets initial OChar in output buffer to zero (null).
 * Dequeue only leads to WRITE_A_READ_A and WRITE_B_READ_B
 * when it reads the end of the current part. */
OlyStatus 
dequeue_from_string_buffer(OlyStringBuffer *strbuf, OChar **dest, const size_t size_in,
        size_t *length )
{
    OlyStatus status = OLY_OKAY;
    OChar **read = NULL, *next = NULL, *copier = (*dest), *stop = ((*dest) + size_in);
    status = is_buffer_empty(strbuf);
    if (status == OLY_WARN_BUFFER_EMPTY)
    {
        strbuf->state = WRITE_A_READ_A;
        (**dest) = (OChar)0;
        return status;
    }
    if ((strbuf->state == WRITE_A_READ_A) || (strbuf->state == WRITE_B_READ_A))
    {
        read = &(strbuf->read_a);
    }
    else
    {
        read = &(strbuf->read_b);
    }
    next = (*read);
    while (((*(copier++) = *(next++)) != 0) && ( copier < stop ))
        ;
    
    if (copier >= (stop - 1))
    {
        status = OLY_ERR_BUFFER_OVERFLOW;
        (**dest) = (OChar)0;
        return status;
    }
    (*length) = (copier - (*dest));
    (*read) += (*length + 1);
    get_higher_end(strbuf, &next);
    if (*read >= next)
    {
#ifdef DEBUG_STRING_BUFFER
        report_values_green(strbuf, __func__);
#endif
        switch(strbuf->state)
        {
        case WRITE_A_READ_A:
            if (strbuf->read_a >= strbuf->write_a)
            {
                strbuf->read_a = strbuf->buffer_start;
                strbuf->write_a = strbuf->buffer_start;
                strbuf->state = WRITE_B_READ_B;
            }
            break;
        case WRITE_B_READ_A:
            if (strbuf->read_a >= strbuf->write_a)
            {
                strbuf->read_a = strbuf->buffer_start;
                strbuf->write_a = strbuf->buffer_start;
                strbuf->state = WRITE_B_READ_B;
            }
            break;
        case WRITE_B_READ_B:
            if (strbuf->read_b >= strbuf->write_b)
            {
                strbuf->read_b = strbuf->buffer_start;
                strbuf->write_b = strbuf->buffer_start;
                strbuf->state = WRITE_A_READ_A;
            }
            break;
        case WRITE_A_READ_B:
            if (strbuf->read_b >= strbuf->write_b)
            {
                strbuf->read_b = strbuf->buffer_start;
                strbuf->write_b = strbuf->buffer_start;
                strbuf->state = WRITE_A_READ_A;
            }
            break;
        }
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
        strbuf->read_a          = NULL;
        strbuf->write_a         = NULL;
        strbuf->read_b          = NULL;
        strbuf->write_b         = NULL;
        strbuf->buffer_end      = NULL;
        free(strbuf->buffer_start);
        strbuf->buffer_start    = NULL;
    }
    if (strbuf != NULL)
    {
        free(strbuf);
    }
}


#ifdef DEBUG_STRING_BUFFER
static char *get_state(OlyStringBuffer *strbuf);

extern char displaybuff[BUFSIZ];
char displaybuff[BUFSIZ];
static void report_values_color(OlyStringBuffer *strbuf, const char *function, unsigned int color);

void 
report_values_color(OlyStringBuffer *strbuf, const char *function, unsigned int color)
{
    sprintf(displaybuff, "f: %s, state: %s, bra: %zu, bwa %zu, brb %zu, bwb %zu\n", function, get_state(strbuf),
        (strbuf->read_a-strbuf->buffer_start),
        (strbuf->write_a-strbuf->buffer_start),
        (strbuf->read_b-strbuf->buffer_start),
        (strbuf->write_b-strbuf->buffer_start));
    print_stdout_char_color( color, BLACK, BRIGHT, displaybuff);
}

static void report_values_color(OlyStringBuffer *strbuf, const char *function, unsigned int color);
void report_values_green(OlyStringBuffer *strbuf, const char *function)
{
    report_values_color(strbuf, function, GREEN);
}

void report_values(OlyStringBuffer *strbuf, const char *function)
{
    report_values_color(strbuf, function, RED);
}

static char *get_state(OlyStringBuffer *strbuf)
{
    char *state;
    switch(strbuf->state)
    {
        case WRITE_A_READ_A:
            state = "WARA";
            break;
        case WRITE_A_READ_B:
            state = "WARB";
            break;
        case WRITE_B_READ_B:
            state = "WBRB";
            break;
        case WRITE_B_READ_A:
            state = "WBRA";
            break;
    }
    return state;
}

#endif 
