/* boundary.c - Oly boundary buffer functions License GPL 2+{{{
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
#include "data_source/pvt_boundary.h"

OlyStatus
open_oly_boundary(char *charset, size_t buffer_max_size, OlyBoundary **bind)
{
    OlyStatus    status = OLY_OKAY;
    UErrorCode   u_status = U_ZERO_ERROR;
    size_t       o_size = 0, c_size = 0, max_characters = 0, min_char_size = 0;
    char        *break_ptr;
    OlyBoundary *oly_bound = omalloc(sizeof(OlyBoundary));
    /* oly boundary uses two buffers:
     * 1. Buffer full of input characters
     * 2. Buffer full of OChars 
     *
     * to use it, drop chars in the input buffer and once it is full it will flush 
     * them. */

    /* Allocate the space for both buffers,
     * then chop it into two spaces according to character size ratios. 
     * Add sizeof(OChar) to accomodate an OChar sized null space to 
     * delineate the two areas. 
     * Consertative and wastes a bit of the end of the buffer.
     * Better that then overflows. */
    
    oly_bound->converter   = ucnv_open( charset, &u_status );
    min_char_size = ucnv_getMinCharSize(oly_bound->converter) ;
    max_characters  = (buffer_max_size / ( min_char_size + sizeof(OChar)));
    o_size          = (( max_characters * sizeof(OChar) ) - sizeof(OChar));
    c_size          = (( max_characters * min_char_size ) - min_char_size);

    /* normally the ICU converter API only uses 6 pointers, 
     * 3 for the outside charset (c_) and 3 for the ICU internal
     * UChar structures (o_).  The flush_break pointers are 
     * there to hold the spot for higher nodes. 
     * because the char buffers are actually pointing to different 
     * character sets, although we only expect to find max_characters characters in it,
     * as they are char pointers we must move it the correct number of chars. */
    oly_bound->c_start = (char *)omalloc( o_size +c_size +min_char_size +sizeof(OChar));
    oly_bound->c_now            = oly_bound->c_start;
    oly_bound->c_end            = ((oly_bound->c_now + 
                ((max_characters*min_char_size)- min_char_size*2)));

    /* OChar buffer
     * unlike the char buffer, the OChar buffer points to its actual character set.
     * Thus, it must only move max_characters forward, as opposed to 
     * o_size. */
    oly_bound->o_now            = (OChar *)((oly_bound->c_end) + min_char_size + 1);
    oly_bound->o_start          = oly_bound->o_now;
    oly_bound->o_end            = ((oly_bound->o_now) + (max_characters-2));
    
    /* make sure the area after each buffer is null. */
    for ( break_ptr = ((oly_bound->c_end) + 1); 
            ( break_ptr < (char *)oly_bound->o_now ); break_ptr++ )
    {
        *break_ptr = '\0' ;
    }
    
    for ( break_ptr = (char *)(oly_bound->o_end); 
            ( break_ptr < (oly_bound->c_start + buffer_max_size )); break_ptr++ )
    {
        *break_ptr = '\0' ;
    }
   
    /* assert same number of characters fit in both buffers. */
    o_size = (size_t)(oly_bound->o_end - oly_bound->o_start);
    c_size = (size_t)((oly_bound->c_end - oly_bound->c_start)/min_char_size);
    assert( o_size == c_size );

    /* assert total sum of size is less than max size. */
    o_size = (size_t)((char *)oly_bound->o_end - (char *)oly_bound->o_start);
    c_size = (size_t)(oly_bound->c_end - oly_bound->c_start);
    assert( ( o_size + c_size ) < buffer_max_size );
    /* o_size and c_size contain exact multiples of the size of their respective character sets. */
    assert( ( o_size % sizeof(OChar) ) == 0 );
    assert( ( c_size % min_char_size ) == 0 );
    (*bind) = oly_bound;

    return status;
};

void
close_oly_boundary(OlyBoundary *bind)
{
    ucnv_close(bind->converter);
    free(bind->c_start);
    free(bind);
    return;
}

OlyStatus
flush_inbound( OlyBoundary *boundary )
{
    OlyStatus status = OLY_OKAY;
    UErrorCode   u_status = U_ZERO_ERROR;
    OChar       *o_start_ptr =  NULL;
    const char  *c_end_ptr   =  boundary->c_now,
                *c_start_ptr =  boundary->c_start;

    o_start_ptr =  boundary->o_start;
    if (c_end_ptr == c_start_ptr)
    {
        status = OLY_WARN_BUFFER_EMPTY;
        return status;
    }

    ucnv_toUnicode( boundary->converter,
        &o_start_ptr, boundary->o_end, &c_start_ptr, c_end_ptr, NULL,
        FALSE, &u_status );

    if (U_FAILURE(u_status))
    {
        fprintf(stderr, "ICU Error: %s.\n", u_errorName(u_status));
        status = OLY_ERR_LIB;
    }
    else
    {
        boundary->c_now = boundary->c_start;
        boundary->o_now = boundary->o_start;
    }
    return status;
}

OlyStatus
flush_outbound( OlyBoundary *boundary )
{
    OlyStatus status = OLY_OKAY;
    UErrorCode  u_status;
    const OChar       *o_start_ptr = boundary->o_start,
                *o_end_ptr   = (boundary->o_now + strlen( boundary->c_now ));
    char        *c_start_ptr = boundary->c_start;
    if (o_end_ptr == o_start_ptr)
    {
        status = OLY_WARN_BUFFER_EMPTY;
        return status;
    }
    
    ucnv_fromUnicode ( boundary->converter, &c_start_ptr, boundary->c_end, 
            &o_start_ptr, o_end_ptr, NULL, FALSE, &u_status );
    if (U_FAILURE(u_status))
    {
        fprintf(stderr, "ICU Error: %s.\n", u_errorName(u_status));
        status = OLY_ERR_LIB;
    }
    return status;
}

OlyStatus
copy_ochar_buffer( OlyBoundary *source, OChar *dest, size_t limit )
{
    u_memcpy(source->o_start, dest, limit );
    return OLY_OKAY;
}

OlyStatus
get_boundary_out_start( OlyBoundary *source, OChar **start )
{
    OlyStatus status = OLY_OKAY;
    (*start) = source->o_start;
    return status;
}

OlyStatus 
put_char_in( OlyBoundary *boundary, const char *s, size_t len)
{
    OlyStatus status = OLY_OKAY;
    size_t available = 0;
    available = ( boundary->c_end - boundary->c_now );
    if (len >= available)
    {
        status = OLY_ERR_BUFFER_OVERFLOW;
        return status;
    }
    strncpy(boundary->c_now, s, len);
    boundary->c_now += len;
    *(boundary->c_now) = '\0';
    (boundary->c_now)++;
    return status;
}

OlyStatus clear_boundary(OlyBoundary *boundary)
{
    OlyStatus status = OLY_OKAY;
    boundary->c_now = boundary->c_start;
    boundary->o_now = boundary->o_start;
    return status;
}

OlyStatus
get_ochar_out( OlyBoundary *boundary, OChar **next, size_t *next_len )
{
    OlyStatus status = OLY_OKAY;
    *next_len = u_strlen(boundary->o_now);
    if (*next_len == 0)
    {
        status = OLY_WARN_BUFFER_EMPTY;
        (*next) = NULL;
        return status;
    }
    *next_len = (u_strlen(boundary->o_now) + 1);
    (*next) = boundary->o_now;

    if ((boundary->o_now + *next_len) < boundary->o_end)
    {
        boundary->o_now += *next_len;
    }
    else 
    {
        status = OLY_WARN_BOUNDARY_RESET;
        *next_len = 0;
        (*next) = NULL;
    }
    return status;
}

OlyStatus
finish_inbound( OlyBoundary *boundary )
{
    OlyStatus status = OLY_OKAY;
    UErrorCode   u_status = U_ZERO_ERROR;
    OChar       *o_start_ptr =  boundary->o_start;
    const char  *c_end_ptr   = (boundary->c_now + strlen( boundary->c_now )),
                *c_start_ptr =  boundary->c_start;

    ucnv_toUnicode( boundary->converter,
        &o_start_ptr, boundary->o_end, &c_start_ptr, c_end_ptr, NULL,
        TRUE, &u_status );

    if (U_FAILURE(u_status))
    {
        fprintf(stderr, "ICU Error: %s.\n",
                u_errorName(u_status));
        status = OLY_ERR_LIB;
    }
    return status;
}

OlyStatus
finish_outbound( OlyBoundary *boundary )
{
    OlyStatus status = OLY_OKAY;
    UErrorCode   u_status = U_ZERO_ERROR;
    const OChar       *o_start_ptr = boundary->o_start,
                *o_end_ptr   = (boundary->o_now + strlen( boundary->c_now ));
    char        *c_start_ptr = boundary->c_start;
    
    ucnv_fromUnicode ( boundary->converter, &c_start_ptr, boundary->c_end, 
            &o_start_ptr, o_end_ptr, NULL, TRUE, &u_status );
    if (U_FAILURE(u_status))
    {
        fprintf(stderr, "ICU Error: %s.\n", u_errorName(u_status));
        status = OLY_ERR_LIB;
    }
    return status;
}

