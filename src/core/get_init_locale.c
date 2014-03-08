/* get_init_locale.c - returns the best match locale for the user. License GPL2+ {{{
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

#include "oly/core.h"
#include "pvt_core.h"

OlyStatus get_init_charset (char *charset[], OlyStatus *status)
{
    int32_t         output_size = 0;
    UErrorCode      u_status  = U_ZERO_ERROR;
    char            *r;
    *status         = OLY_OKAY;
    
    output_size = uloc_getBaseName(NULL, r, OLY_SMALL_BUFFER, &u_status);
    if (U_FAILURE(u_status)) 
    {
        printf("uloc_getBaseName failed: %s\n",  u_errorName(u_status));
        *status = OLY_ERR_LIB;
    }
    if (( output_size > 0 ) &&  ( *status == OLY_OKAY ))
    {
        *locale = (char *)xmalloc( (output_size + 1) * ( sizeof(char) ));
        strncpy(*locale, result, output_size);
        *((*locale) + output_size) = '\0';
    } 
    else if (*status == OLY_OKAY) 
    {
            *status = OLY_ERR_INIT;
    }
    return *status;
}
