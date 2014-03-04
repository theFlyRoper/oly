/* level_indent.c - returns indentation for this level. {{{
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

#include <sys/types.h>
#include <unistd.h>

#include "oly/oly_dev.h"

char *level_indent(const int level)
{
    int i = 0, j = 0;
    const int tabsize = 2;
    char indent[BUFSIZ],
         *indent_ptr;
    indent_ptr = indent;
    for ( i = 0; (i < level && (i*tabsize) < BUFSIZ); i++) 
    {
        for (j = 0; (j < tabsize); j++, indent_ptr++) {
            *indent_ptr = ' ';
        }
    }
    *indent_ptr = '\0';
    return indent;
}

