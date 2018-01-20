/* flag_check.c - function to retrieve current resource type. {{{
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

#define URES_TREE_DEBUG 1

UResType flag_check(UResourceBundle *res, const res_disp_flag *flag)
{
    UResType restype = ures_getType(res);
    if (flag->all == 1)
    {
        return restype;
    } 
    else if (restype == URES_ALIAS && flag->aliases == 1) 
    {
        return restype;
    }
    else if (restype == URES_ARRAY && flag->arrays == 1) 
    {
        return restype;
    }
    else if (restype == URES_BINARY && flag->binaries == 1)
    {
        return restype;
    }
    else if (restype == URES_INT && flag->integers == 1)
    {
        return restype;
    }
    else if (restype == URES_INT_VECTOR && flag->vectors == 1)
    {
        return restype;
    }
    else if (restype == URES_STRING && flag->strings == 1)
    {
        return restype;
    }
    else if (restype == URES_TABLE && flag->tables == 1)
    {
        return restype;
    }
    else {
        return URES_NONE;
    }
    
}

