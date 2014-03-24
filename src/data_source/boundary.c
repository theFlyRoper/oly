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

#include "oly/olytypes.h"
#include "data_source/pvt_boundary.h"

OlyBoundary *
open_oly_boundary(char *charset, size_t buffer_max_size, OlyStatus *status)
{
    UErrorCode   u_status = U_ZERO_ERROR;
    size_t       o_size = 0, c_size = 0, max_characters = 0, min_char_size = 0,
                 buffer_max_size = get_max_buffer_size(ds, status);
    char        *break_ptr;
    OlyBoundary *oly_bound = omalloc(sizeof(OlyBoundary));
    /* oly boundary uses two buffers:
     * 1. Buffer full of input characters
     * 2. Buffer full of OChars 
     *
     * to use it, drop chars in the input buffer and once it is full it will flush
     * them.
     */

    /* Allocate the space for both buffers,
     * then chop it into two spaces according to character size ratios. 
     * Add sizeof(OChar) to accomodate an OChar sized null space to 
     * delineate the two areas. 
     * Consertative and wastes a bit of the end of the buffer.
     * Better that then overflows.
     */
    oly_bound->converter   = ucnv_open( get_data_charset(ds), &u_status );
    min_char_size = ucnv_getMinCharSize(oly_bound->converter) ;
    max_characters      = (buffer_max_size / ( min_char_size + sizeof(OChar)));
    o_size          = (( max_characters * sizeof(OChar) ) - sizeof(OChar));
    c_size          = (( max_characters * min_char_size ) - min_char_size);

    /* normally the ICU converter API only uses 6 pointers, 
     * 3 for the outside charset (c_) and 3 for the ICU internal
     * UChar structures (o_).  The flush_break pointers are 
     * there to hold the spot for higher nodes. 
     * 
     * because the char buffers are actually pointing to different 
     * character sets, although we only expect to find max_characters characters in it,
     * as they are char pointers we must move it the correct number of chars. */
    oly_bound->c_now = (char *)omalloc( o_size +c_size +min_char_size +sizeof(OChar));
    oly_bound->c_start          = oly_bound->c_now;
    oly_bound->c_end            = ((oly_bound->c_now + 
                ((max_characters*min_char_size)- min_char_size*2)));
    oly_bound->c_flush_break    = oly_bound->c_now;

    /* OChar buffer
     * unlike the char buffer, the OChar buffer points to its actual character set.
     * Thus, it must only move max_characters forward, as opposed to 
     * o_size. */
    oly_bound->o_now            = (OChar *)((oly_bound->c_end) + min_char_size + 1);
    oly_bound->o_start          = oly_bound->o_now;
    oly_bound->o_end            = ((oly_bound->o_now) + (max_characters-2));
    oly_bound->o_flush_break    = oly_bound->o_now;
    
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

    return oly_bound;
};

