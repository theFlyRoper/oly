/* main for csv importer. License GPL2+ {{{
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <assert.h>

#include "oly/core.h"
#include "oly/output.h"
#include "oly/break_rules.h"
#include "oly/globals.h"
#include "pvt_core.h"
/* MAIN */
int
main( int argc, char **argv )
{
    OlyStatus        status  = OLY_OKAY;
    OlyTagType       tag_type = OLY_TAG_TYPE_UNSET;
    char            *locale = NULL, *encoding = NULL, *locdir = PKGDATADIR; 
    const char      *test_tags[] = { "NULL", "Null", "null", "~", NULL },
                    *tag_type_list[] = {
                        "OLY_TAG_SCALAR_NULL",
                        "OLY_TAG_SCALAR_BOOL",
                        "OLY_TAG_SCALAR_INT",
                        "OLY_TAG_SCALAR_UINT",
                        "OLY_TAG_SCALAR_FLOAT",
                        "OLY_TAG_SCALAR_INFINITY",
                        "OLY_TAG_SCALAR_NOT_A_NUMBER",
                        "OLY_TAG_SCALAR_STRING"
                    };
    OChar            test_buffer[BUFSIZ];
    int              i = 0;
    UErrorCode u_status = U_ZERO_ERROR;
#ifdef OLYDEV
    res_disp_flag   flag;
#endif /* OLYDEV */

    status = init_oly(argv[0], locdir, encoding, locale);
    for ( i = 0; (test_tags[i] != NULL); i++ )
    {
        u_strFromUTF8( test_buffer, BUFSIZ, NULL, test_tags[i], -1, &u_status );
        if (U_FAILURE(u_status))
        {
            printf("Error test_buffer %s.\n",
                    u_errorName(u_status));
            exit(EXIT_FAILURE);
        }
        status = infer_simple_tag(test_buffer, &tag_type);
        printf("Record: %s, tag type: %s\n", test_tags[i], tag_type_list[tag_type]);
    }

#ifdef OLYDEV
    init_res_disp_flag(&flag);
    list_package_locales(OLY_RESOURCE);
/*    list_table_resources(get_resource_data(oly->data), &flag, 0); */
#endif /* OLYDEV */
  
    HANDLE_STATUS_AND_DIE(status);
    return EXIT_SUCCESS;
}


