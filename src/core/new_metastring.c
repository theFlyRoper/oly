/* new_metastring.c - Constructor for a metastring struct. License GPL2+ {{{
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

OlyMetastring *
new_metastring(const char *input)
{
    size_t           len    = (strlen(input)+1);
    OlyMetastring   *retval = (OlyMetastring *)xmalloc(sizeof(OlyMetastring));
    OChar           *o = xmalloc(sizeof(OChar)*len);
    retval->internal = xstrdup(input);
    retval->display = u_uastrncpy((UChar *)o, (const char *)input, len);
    return retval; 
}
