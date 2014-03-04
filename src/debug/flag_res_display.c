/* flag_res_display.c - function to retrieve current resource type. {{{
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

#include <unicode/umachine.h>
#include <sys/types.h>
#include <unistd.h>

#include "oly/oly_dev.h"

#define URES_TREE_DEBUG 1

void flag_res_display(UResourceBundle *res, const res_disp_flag *flag, const int level)
{
    char    holder[BUFSIZ], *hold_ptr;
    int32_t len = BUFSIZ;
    UErrorCode u_status = U_ZERO_ERROR;
    UResType type = flag_check(res, flag);
    switch (type) {
    case URES_ALIAS:
        hold_ptr = ures_getUTF8String(res, &holder, &len, TRUE, &u_status);
        if (U_FAILURE(u_status)) {
            fprintf(stderr, "Err in flag_res_display, URES_ALIAS: %s\n",
                    u_errorName(u_status));
            exit(1);
        }
        printf("%s\n",hold_ptr);
        break;
    case URES_ARRAY:
        list_array_resources(res, flag, (level+1));
        break;
    case URES_BINARY:
        printf("[binary data]\n");
        break;
    case URES_INT:
        len = ures_getInt(res, &u_status);
        if (U_FAILURE(u_status)) {
            fprintf(stderr, "Err in flag_res_display, URES_INT: %s\n",
                    u_errorName(u_status));
            exit(1);
        }
        printf("%i\n",len = ures_getInt(res, &u_status));
        break;
    case URES_INT_VECTOR:
        printf("Size: %i\n", ures_getSize(res));
        break;
    case URES_STRING:
        hold_ptr = ures_getUTF8String(res, &holder, &len, TRUE, &u_status);
        if (U_FAILURE(u_status)) {
            fprintf(stderr, "Err in flag_res_display, URES_STRING: %s\n",
                    u_errorName(u_status));
            exit(1);
        }
        printf("%s\n",hold_ptr);
        break;
    case URES_TABLE:
        list_table_resources(res, flag, (level+1));
        break;
    default:
        break;
    }
}

