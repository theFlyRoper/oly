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

#include "oly/core.h"
#include "oly/state.h"
#include "oly/resources.h"

oly_status 
check_liberror(oly_state *s){
    OChar *errtext = NULL;
    oly_status status;
#ifdef HAVE_UNICODE_USTDIO_H
    if (U_FAILURE(s->lib_status)) {
        errtext = cstr_to_ostr( &status, (const char *)u_errorName(s->lib_status));
        set_state_message( s, cstr_to_ostr(&status, "check_liberror: problem.\n"));
        s->status = OLY_ERR_LIB;
    }
#endif /* HAVE_UNICODE_USTDIO_H */
    return s->status;
}

