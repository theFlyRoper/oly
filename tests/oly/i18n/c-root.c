/* root test file for resources. License GPL2+ {{{
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

#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif

#include <unicode/ustdio.h>
#include <unicode/ures.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "oly/common.h"
#include "oly/core.h"
#include "oly/globals.h"

/* MAIN */
int
main( int argc, char **argv ){
    int32_t              len        = 0;
    ochar               *liner;
    Oly                 *oly=new_oly();
    char                *locale     = "root", *locdir=(char*)TEST_PKGDATADIR;
    char                *program_name      = argv[0];
    int                  i=1;
    UErrorCode           u_status   = U_ZERO_ERROR; 
    if (init_oly(oly, argv[0], locdir, NULL, locale) != OLY_OKAY) {
        perror("Initialization failed\n");
    };
    
    liner = ures_getStringByKey(oly->data->resource, "OlyUsage", &len, &u_status );
    u_file_write(liner, len, u_stdout);
    
    if (U_SUCCESS(u_status)) {
        return EXIT_SUCCESS;
    }
    else {
        return EXIT_FAILURE;
    }
}


