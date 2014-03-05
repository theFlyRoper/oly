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

/* MAIN */
int
main( int argc, char **argv )
{
    int32_t         len,optc  = 0, i=1;
    char            rules_file_name[] = "./tests/data/supabreak.txt",
                    c_line[] = "Rusty,\"Block, Head\", Blomster, \"3.1415,92,9\"",
                    c_line2[] = "\"Lorem, ipsum\",Blomster,\"1.41\",Yorgle",
                    *program_name      = argv[0], 
                    *resource_file = (char*)OLY_RESOURCE;
    ochar           line[BUFSIZ], *liner;     /* main buffer */
    char            *optionError, *locale, *dbg_var;
    oly_status      o_status  = OLY_OKAY;
    oly_state       main_state;
    UErrorCode      u_status  = U_ZERO_ERROR; 
    oly_resource  *OlyResources;
    UBreakIterator  *boundary;
#ifdef OLYDEV
    res_disp_flag   flag;
#endif /* OLYDEV */

    atexit (close_oly);
    if (init_state(&main_state) != OLY_OKAY) {
        printf("Init state failed\n");
    };
    program_name      = argv[0];

    /* u_setDataDirectory tells ICU where to look for custom app data.  It is not needed
    * for the internal app data for ICU, which lives in a shared library. */
    u_setDataDirectory(PKGDATADIR);
    printf("Data directory set to : %s\n", PKGDATADIR);
    if (get_default_locale (&locale, &o_status) != OLY_OKAY) {
        printf("DEFAULT LOCALE: %i\n", o_status);
    }
#ifdef OLYDEV
    printf("\n-- top of program, locale is : %s\n", locale);
#endif /* OLYDEV */
    init_all(locale);
    OlyResources = ures_open(resource_file, locale, &u_status); 
    if (U_FAILURE(u_status)) {
        printf("Could not open! Errmsg: %s\n", u_errorName(u_status));
    }
    boundary = get_rules(rules_file_name, u_status);
    if (U_FAILURE(u_status)) {
        printf("Could not open resource %s, error %s, locale: %s\n", 
                    resource_file, u_errorName(u_status), locale);
    }
    
    printf("Examining: %s\n", c_line);
    u_uastrcpy(line, c_line);

    ubrk_setText(boundary, line, u_strlen(line), &u_status);
#ifdef OLYDEV
    init_res_disp_flag(&flag);
    list_package_locales(OLY_RESOURCE);
    list_table_resources(OlyResources, &flag, 0);
#endif /* OLYDEV */

  
    if (U_FAILURE(u_status)) {
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }
}


