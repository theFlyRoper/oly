/* get_error_message.c - get info about state.  License GPL2+ {{{
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
#include "oly/oly.h"

OChar *
get_error_message( OlyState *state, OlyStatus *err_status )
{   
    OChar *message = NULL;
    int32_t len = 0;
#ifdef HAVE_UNICODE_UMSG_H
    message = ures_getStringByIndex(errors, 
            ((*err_status) + OLY_ERR_OFFSET), &len, &u_status);
    if (U_FAILURE(u_status)) {
        printf("Could not load error %i, offset %i, status: %s\n",
                *err_status, OLY_ERR_OFFSET, u_errorName(u_status));
        return NULL;
    }
#endif /* HAVE_UNICODE_UMSG_H */
    return message; 
}
