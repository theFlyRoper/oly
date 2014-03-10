/* check_liberror.c - check the UErrorCode and holler if it is suboptimal. License GPL2+ {{{
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

#include "oly/state.h"
#include "oly/resources.h"
#include "pvt_state.h"

OlyStatus 
check_liberror(OlyState *s){
    OChar       *errtext;
    size_t       errtext_size;
    OlyStatus    status;

    /* s->status = OLY_OKAY; */
#ifdef HAVE_UNICODE_USTDIO_H
    errtext_size = strlen(u_errorName(s->lib_status));
    if ((s->lib_status != U_ZERO_ERROR) && (U_FAILURE(s->lib_status))) 
    {
        buffer_message( s, ERROR_ACTION , cstr_to_ostr( errtext, errtext_size,
                (const char *)u_errorName(s->lib_status), &status) );
        s->status = OLY_ERR_LIB;
    }
    else if ((s->lib_status != U_ZERO_ERROR) && (U_SUCCESS(s->lib_status)))
    {
        buffer_message( s, WARNING_ACTION , cstr_to_ostr( errtext, 
                    errtext_size, (const char *)u_errorName(s->lib_status),
                    &status) );
        s->status = OLY_WARN_LIB;
    }
    else
    {
        s->status = OLY_OKAY;
    }
#endif /* HAVE_UNICODE_USTDIO_H */
    return s->status;
}

