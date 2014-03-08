/* get_init_charset.c -- License GPL2+ {{{
 * Copyright (C) 2012 Oly Project
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
    char            *result_ptr;
    *status         = OLY_OKAY;
    
    result_ptr = ucnv_getDefaultName();
    output_size = strlen(result_ptr);
    if (( output_size > 0 ) &&  ( *status == OLY_OKAY ))
    {
        *charset = (char *)xmalloc( (output_size) * ( sizeof(char) ));
        strncpy(*charset, result_ptr, output_size);
        *((*charset) + output_size) = '\0';
    } 
    else if (*status == OLY_OKAY) 
    {
            *status = OLY_ERR_INIT;
    }
    return *status;
}
