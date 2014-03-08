/* clear_liberror.c - check the UErrorCode and holler if it is suboptimal. License GPL2+ {{{
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

OlyStatus 
clear_liberror(OlyState *s)
{
#ifdef HAVE_UNICODE_USTDIO_H
    s->lib_status = U_ZERO_ERROR ;
    if (s->status == OLY_ERR_LIB)
    {
        s->status = OLY_OKAY;
    }
#endif /* HAVE_UNICODE_USTDIO_H */
    return s->status;
}

