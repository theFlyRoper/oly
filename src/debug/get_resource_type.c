/* get_resource_type.c - function to retrieve current resource type. {{{
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

char *get_resource_type(UResourceBundle *res)
{ 
    char *types[] = {"URES_ALIAS", "URES_ARRAY", "URES_BINARY",
        "URES_INT", "URES_INT_VECTOR", "URES_NONE", 
        "URES_STRING", "URES_TABLE"};
    char *retval;
    switch (ures_getType(res)) {
    case URES_ALIAS:
        retval = types[0];
        break;
    case URES_ARRAY:
        retval = types[1];
        break;
    case URES_BINARY:
        retval = types[2];
        break;
    case URES_INT:
        retval = types[3];
        break;
    case URES_INT_VECTOR:
        retval = types[4];
        break;
    case URES_NONE:
        retval = types[5];
        break;
    case URES_STRING:
        retval = types[6];
        break;
    case URES_TABLE:
        retval = types[7];
        break;
    default:
        retval = types[5];
        break;
    }
    return retval;
}

