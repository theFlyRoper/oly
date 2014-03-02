/* ifelse_res_display.c - function to retrieve current resource type. {{{
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

void ifelse_res_display(UResourceBundle *res, const res_disp_flag flag, const int level)
{
    fprintf(stderr, "Doin my thing at the top of ifelse resdisplay\n");
    if (flag.all == 1) {
        display_resource_type(res, level);
        fprintf(stderr, "Boy that was somthin\n");
    } else {
        switch (ures_getType(res)) {
        case URES_ALIAS:
            if (flag.aliases == 1) 
            {
                display_resource_type(res, level);
            }
            break;
        case URES_ARRAY:
            if (flag.arrays == 1)
            {
                display_resource_type(res, level);
            }
            break;
        case URES_BINARY:
            if (flag.binaries == 1)
            {
                display_resource_type(res, level);
            }
            break;
        case URES_INT:
            if (flag.integers == 1)
            {
                display_resource_type(res, level);
            }
            break;
        case URES_INT_VECTOR:
            if (flag.vectors == 1)
            {
                display_resource_type(res, level);
            }
            break;
        case URES_STRING:
            if (flag.strings == 1)
            {
                display_resource_type(res, level);
            }
            break;
        case URES_TABLE:
            if (flag.tables == 1)
            {
                display_resource_type(res, level);
            }
            break;
        default:
            break;
        }
    }
}

