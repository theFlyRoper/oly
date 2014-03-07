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
    Oly             *oly=new_oly();
    oly_status      status  = OLY_OKAY;
#ifdef OLYDEV
    res_disp_flag   flag;
#endif /* OLYDEV */
    if (init_oly(oly, progval, locdir, NULL, locale) != OLY_OKAY) {
        perror("Initialization failed\n");
    };
#ifdef OLYDEV
    init_res_disp_flag(&flag);
    list_package_locales(OLY_RESOURCE);
    list_table_resources(oly->data, &flag, 0);
#endif /* OLYDEV */
  
    if (status != OLY_OKAY) {
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }
}


