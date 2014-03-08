/* open_state_resource.c - allocates an Oly object and returns a pointer to it.  License GPL2+ {{{
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

OlyStatus 
open_state_resource( oly_state *res, resource_data *master)
{
#ifdef HAVE_UNICODE_URES_H
    UErrorCode  u_status;
#endif /* HAVE_UNICODE_URES_H */
    assert( res != NULL && res_dir != NULL );
    res->status = OLY_OKAY;
#ifdef HAVE_UNICODE_URES_H
    res->resource = (resource_data *)ures_open(res_dir, res->locale, &u_status);
    if (U_FAILURE(u_status))
    {
        printf("Resource file error. Status: %s.\n",
                u_errorName(u_status));
        res->status = OLY_ERR_LIB;
    }
#endif /* HAVE_UNICODE_URES_H */
    return res->status; 
}
