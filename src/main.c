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

#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif

#include "oly/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <assert.h>

#include "oly/state.h"
#include "oly/core.h"
#include "oly/output.h"
#include "oly/list.h"
#include "oly/syntax.h"
#include "oly/oly.h"
#include "oly/builtin.h"
#include "oly/break_rules.h"

/* MAIN */
int
main( int argc, char **argv ){
  Oly             *oly      = oly_new ();
  int32_t         len,optc  = 0;
  char            rules_file_name[] = "./tests/data/supabreak.txt";
  ochar           line[BUFSIZ];     /* main buffer */
  ochar           *liner;
  char            *optionError;     
  char            *locale;
  char            *dbg_var;
  char            c_line[] = "Rusty,\"Block, Head\", Blomster, \"3.1415,92,9\"";
  char            c_line2[] = "\"Lorem, ipsum\",Blomster,\"1.41\",Yorgle";
  int             i=1;
  oly_status      o_status  = OLY_OKAY;
  UErrorCode      u_status  = U_ZERO_ERROR; 
  UBreakIterator  *boundary;

  atexit (close_oly);
  program_name      = argv[0];

  /* u_setDataDirectory tells ICU where to look for custom app data.  It is not needed
   * for the internal app data for ICU, which lives in a shared library. */
  u_setDataDirectory(LOCALEDIR);
  printf("Data directory set to : %s\n", LOCALEDIR);
  locale = oget_user_locale(); 
  locale = "root";
#ifdef OLYDEV
  printf("\n-- top of program, locale is : %s\n", locale);
#endif /* OLYDEV */
  printf("Stuff before init all.\n");
  init_all(oly, locale);
  OlyResources = ures_open(OLY_RESOURCE, locale, &u_status); 
  printf("Stuff after init all.\n");
#ifdef OLYDEV
  printf("\n-- after init --\n");
#endif /* OLYDEV */

  boundary = get_rules(rules_file_name, u_status);
  if (U_FAILURE(u_status)) {
    printf("Could not open!\n");
  }
  
  printf("Examining: %s\n", c_line);
  u_uastrcpy(line, c_line);

  ubrk_setText(boundary, line, u_strlen(line), &u_status);
  len = NULL;

  i = BUFSIZ;
  
#ifdef OLYDEV
    list_package_locales(OLY_RESOURCE);
    printf("OlyResources package type: ");
    switch (ures_getType(OlyResources)) {
    case URES_NONE:
        printf("URES_NONE\n");
        break;
    case URES_STRING:
        printf("URES_STRING\n");
        break;
    case URES_BINARY:
        printf("URES_BINARY\n");
        break;
    case URES_TABLE:
        printf("URES_TABLE\n");
        break;
    case URES_ALIAS:
        printf("URES_ALIAS\n");
        break;
    case URES_INT:
        printf("URES_INT\n");
        break;
    case URES_ARRAY:
        printf("URES_ARRAY\n");
        break;
    case URES_INT_VECTOR:
        printf("URES_INT_VECTOR\n");
        break;
    default:
        printf("NONE OF THE ABOVE\n");
        break;
    }
    list_bundle_resources(OlyResources);
#endif /* OLYDEV */
    get_i18n_errstring(&line, &i, OLY_CONTINUE);
  u_file_write(line, len, u_stdout);

  
  if (U_FAILURE(u_status)) {
    return EXIT_FAILURE;
  } else {
    return EXIT_SUCCESS;
  }
}


