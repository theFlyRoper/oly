/* close_resource.c - Close a resource object License GPL2+ {{{
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
#include "oly/core.h"
#include "oly/resources.h"

void
close_resource(OlyResource *res)
{
    assert( res != NULL );
    if (res->resource != NULL) 
    {
#ifdef HAVE_UNICODE_USTDIO_H
        ures_close((UResourceBundle *)(res->resource));
#endif /* HAVE_UNICODE_USTDIO_H */
    }
    if (res->locale != NULL) 
    {
        close_metastring(res->locale);
    }
    if (res->charset != NULL) 
    {
        close_metastring(res->charset);
    }
    XFREE(res);

    return OLY_OKAY; 
}
