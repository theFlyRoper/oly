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

#include <unicode/ustdio.h>
#include <unicode/ustring.h>
#include <unicode/ubrk.h>
#include <unicode/udata.h>
#include <unicode/ures.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <assert.h>

#include "common.h"
#include "output.h"
#include "error.h"
#include "list.h"
#include "syntax.h"
#include "oly.h"
#include "builtin.h"
#include "loader.h"
#include "break_rules.h"
/* u_stdout, u_stdin and u_stderr and program_name are defined in error.c */

/* MAIN */
int
main( int argc, char **argv ){
  Oly             *oly      = oly_new ();
  int32_t         len,optc  = 0;
  char            rules_file_name[] = "./tests/data/supabreak.txt";
  OChar           line[BUFSIZ];     /* main buffer */
  OChar           *liner;
  char            *optionError      = NULL;     
  char            *locale           = NULL;
  char            *dbg_var          = NULL;
  char            c_line[] = "Rusty,\"Block, Head\", Blomster, \"3.1415,92,9\"";
  char            c_line2[] = "\"Lorem, ipsum\",Blomster,\"1.41\",Yorgle";
  int             i=1;
  Oly_Status      o_status  = OLY_OKAY;
  UErrorCode      u_status  = U_ZERO_ERROR; 
  UBreakIterator  *boundary;
  UResourceBundle *OlySubresource;

  atexit (close_oly);
  program_name      = argv[0];

  /* u_setDataDirectory tells ICU where to look for custom app data.  It is not needed
   * for the internal app data for ICU, which lives in a shared library. */
  u_setDataDirectory(LOCALEDIR);
#ifdef OLYDEV
/* printf("\n-- top of program --\n");
  list_icu_langs(); */
#endif /* OLYDEV */
#ifdef OLYDEV
/*  printf("\n-- after init --\n");
  list_package_locales(NULL); */
#endif /* OLYDEV */

  init_all(oly, locale);
  boundary = get_rules(rules_file_name, u_status);
  if (U_FAILURE(u_status)) {
    printf("Could not open!\n");
  }
  
  OlyResources = ures_open("oly_lang", locale, &u_status); 
  /* locale = oget_user_locale(); */
  printf("Examining: %s\n", c_line);
  u_uastrcpy(line, c_line);

  ubrk_setText(boundary, line, u_strlen(line), &u_status);
  len = NULL;

  liner = ures_getStringByKey(OlyResources, "OlyUsage", &len, &u_status );
  u_file_write(liner, len, u_stdout);
  
  if (U_FAILURE(u_status)) {
      return EXIT_FAILURE;
  } else {
    return EXIT_SUCCESS;
  }
}


