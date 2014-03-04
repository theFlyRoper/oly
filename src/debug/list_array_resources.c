/* list_array_resources.c - Debug function to print indexes from an icu array resource. {{{
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

void list_array_resources(UResourceBundle *res, const res_disp_flag *flag, const int level)
{ 
    UErrorCode      u_status = U_ZERO_ERROR;
    int32_t         i = 0;
    UResourceBundle *subres = NULL;
    
    for (i = 0; i<ures_getSize(res); i++) {
        subres = ures_getByIndex (res, i, subres, &u_status);
        if (U_FAILURE(u_status)) {
            fprintf(stderr, "Err: %s\n",
                    u_errorName(u_status));
            exit(1);
        }
        if (URES_NONE != flag_check(subres, flag)) {
            printf("%s%8i - %s: ", level_indent(level), i, get_resource_type(subres));
            flag_res_display(subres, flag, level);
        }
    }
    printf("\n");
    ures_close(subres);
}

