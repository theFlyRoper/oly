/* oly_fatal - print error number, enum value and word "FATAL:" in front. License GPL2+ {{{
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

#ifdef HAVE_UNICODE_USTDIO_H
#include <unicode/utypes.h>
#endif

#include "oly/core.h"

/* This function is for emergencies, initialization errors, basic functions
 * and suchlike. It is not for general use.  Use the oly_state struct and 
 * its supporting functions for that.
 *
 * If you must use this, add the function to the list here:
 *   src/core/xmalloc.c
 */

void 
oly_fatal_replace(oly_status olyerr, int liberr)
{
    ochar *message = NULL;
    if (u_stderr != NULL) 
    {
        fprintf(stderr, "Oly Error Number: %i -- Could not find u_stderr, exiting...",olyerr);
    } else {
        fprintf(stderr, "Oly Error Number: %i -- Could not find u_stderr, exiting...",olyerr);
    }

    exit(EXIT_FAILURE);
}

