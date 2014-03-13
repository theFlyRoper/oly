/* list_table_resources.c - Debug function to print keys from an ICU table resource. {{{
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

#include <unicode/ustdio.h>
#include <unicode/uloc.h>
#include <unicode/ures.h>

#include <sys/types.h>
#include <unistd.h>

#include "oly/oly_dev.h"

#define URES_TREE_DEBUG 1

void list_table_resources(UResourceBundle *res, 
        const res_disp_flag *flag, 
        const int level)
{ 
    UErrorCode      u_status = U_ZERO_ERROR;
    res_disp_flag   *inner_flag = (res_disp_flag *)flag;
    UResourceBundle *subres = NULL;
    char            indent_buffer[BUFSIZ];
    ures_resetIterator (res);

    if (flag == NULL)
    {
        init_res_disp_flag(inner_flag);
    }
    while (ures_hasNext (res)) {
        subres = ures_getNextResource (res, subres, &u_status);
        if (U_FAILURE(u_status)) {
            fprintf(stderr, "Err: %s\n",
                    u_errorName(u_status));
            exit(1);
        }
        printf("%s%s - %s\n",
                level_indent(indent_buffer, BUFSIZ, level), 
                ures_getKey(subres),  get_resource_type(subres));
            flag_res_display(subres, inner_flag, level);
    }
    ures_close(subres);
}

