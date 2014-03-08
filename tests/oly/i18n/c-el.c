/* greek test file for resources. License GPL2+ {{{
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
#include "oly/oly.h"
#include "oly/globals.h"

/* MAIN */
int
main( int argc, char **argv ){
    int32_t         len       = 0;
    OChar           *liner;
    char            *locale           = "el";
    int             i=1;
    UErrorCode      u_status  = U_ZERO_ERROR; 
    OChar           *program_name = (OChar *)argv[0];
    OlyResource      *OlyResources ;
    if (U_FAILURE(u_status)) {
        printf("Could not open! status: %s\n", u_errorName(u_status));
    }

    /* u_setDataDirectory tells ICU where to look for custom app data.  It is not needed
    * for the internal app data for ICU, which lives in a shared library. */
    u_setDataDirectory(TEST_PKGDATADIR);
    OlyResources = ures_open(OLY_RESOURCE, locale, &u_status);

    u_init(&u_status);
    init_io(locale, NULL);

    liner = ures_getStringByKey(OlyResources, "OlyUsage", &len, &u_status );
    u_file_write(liner, len, u_stdout);
    
    return u_status;
}


