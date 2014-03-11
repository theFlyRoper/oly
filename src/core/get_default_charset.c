/* get_default_charset.c - get the charset for an oly object {{{
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

/* The resource data charset for oly serves as the default charset throughout,
 * since the Oly object is the primary object throughout the program. */
#include "oly/common.h"
#include "sys/types.h"
#include "oly/core.h"
#include "pvt_core.h"
OChar *get_default_charset( Oly *oly )
{
    return get_charset(oly->data);
}
