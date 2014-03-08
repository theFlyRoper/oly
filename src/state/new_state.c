/* new_state.c - allocates a state object and returns a pointer to it.  License GPL2+ {{{
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

oly_state *
new_state( oly_resource *master )
{
#ifdef HAVE_UNICODE_URES_H
    UErrorCode u_status = U_ZERO_ERROR;
#endif /* HAVE_UNICODE_URES_H */
    ures_resetIterator (master);
    assert( master != NULL );
    oly_state *state = (oly_state *)xmalloc(sizeof(oly_state));
    state->output = (ochar *)xcalloc(BUFSIZ, sizeof(ochar));
    state->status = OLY_OKAY;
#ifdef HAVE_UNICODE_URES_H
    state->messages = ures_getByKey((UResourceBundle *)get_resource_data(master), 
            "OlyErrors", NULL, &u_status);
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
