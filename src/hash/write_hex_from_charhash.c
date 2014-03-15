/* write_hex_from_charhash.c - Write out char hex bits. Assumes ascii. License GPL2+ {{{
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
#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif

#include "oly/common.h"

#include "oly/core.h"
#include "oly/hash.h"

/* only writes the hash itself.  Does not follow with anything else. */
OlyStatus
write_hex_from_charhash (FILE *f, const charhash c, OlyState *state)
{
    int  i=0;
    for ( i = 0; (i<(OLY_HASH_BITS/CHAR_BIT)); i += sizeof(char) )
    {
        if ( fprintf(f, "%02x", c[i]) <= 0 )
        {
            return OLY_ERR_FILEIO;
        };
    }
    return OLY_OKAY;
}

OlyStatus
print_hex_from_charhash (const charhash c, OlyState *state)
{
    OlyStatus status = write_hex_from_charhash(stdout, c, state);
    fprintf(stdout, "\n");
    return status;
}

