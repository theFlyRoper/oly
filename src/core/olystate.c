/* olystate.c - State access functions License GPL2+ {{{
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
#include <ctype.h>
#include <assert.h>
#include "oly/core.h"
#include "oly/state.h"
#include "pvt_state.h"

OlyState *
new_state( OlyResource *master )
{
#ifdef HAVE_UNICODE_URES_H
    UErrorCode u_status = U_ZERO_ERROR;
    OlyState *state = (OlyState *)omalloc(sizeof(OlyState));
#endif /* HAVE_UNICODE_URES_H */
    assert( master != NULL );
    state->msgbuf_start = (OChar *)ocalloc(BUFSIZ, sizeof(OChar));
    state->msgbuf_end = state->msgbuf_start;
    state->status = OLY_OKAY;
#ifdef HAVE_UNICODE_URES_H
    state->lib_status = u_status ;
    if (U_FAILURE(u_status)) 
    {
        printf("New state error.  Status: %s.\n",
                u_errorName(u_status));
        state->status = OLY_ERR_LIB;
    }
#endif /* HAVE_UNICODE_URES_H */
    return state; 
}

OlyStatus
set_status(OlyState *state, const OlyStatus status)
{
    state->status = status;
    return OLY_OKAY;
}

OlyStatus
get_status(OlyState *s){
  return s->status;
}

