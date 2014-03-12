/* get_state_message.c - Retrieve the message for this state.  License GPL2+ {{{
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
#include "oly/core.h"
#include "oly/state.h"
#include "pvt_state.h"

/* this function retrieves the message pointer.  It does not copy or allocate any space. */
OChar *
get_state_message( OlyState *state)
{
#ifdef HAVE_UNICODE_URES_H
    UErrorCode u_status = U_ZERO_ERROR;
    int len = 0;
    OChar* result = (OChar *)ures_getStringByIndex( state->messages, 
            (state->status + abs(OLY_STATUS_MIN)), &len, &u_status );
        
    state->lib_status = u_status ;
    if (U_FAILURE(u_status)) 
    {
        printf("Unable to get state message.  Status: %s.\n",
                u_errorName(u_status));
        state->status = OLY_ERR_LIB;
    }
#endif /* HAVE_UNICODE_URES_H */
    return result; 
}
