/* init_all-inl.h -- perform initial cleaning of environment and initialize ICU  License GPL2+ {{{
 * Copyright (C) 2012 Oly Project
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

#include <unicode/uclean.h>
#include <unicode/ustdio.h>
#include <unicode/ustring.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <assert.h>

#include "oly/common.h"
#include "oly/core.h"
#include "oly/state.h"
#include "oly/list.h"
#include "oly/syntax.h"
#include "oly/oly.h"


oly_status
init_all (Oly *oly, char *locale) {
  UErrorCode      u_status = U_ZERO_ERROR; 
  oly_status      o_status = OLY_OKAY;
  clean_io_open();
  oly_result_clear (oly);
  
  /* u_stderr, u_stdout, u_stdin */
  init_io(locale, NULL);
  
  if ( cleanenv() != OLY_OKAY )
    oly_fatal (oly_result_get(oly));

  /* Initialize ICU */
  u_init(&u_status);
  if (U_FAILURE(u_status)) {
      oly_fatal (oly_result_get(oly));
  }
  if (U_FAILURE(u_status)) {
    printf("Could not open! status: %s. Exiting...\n", u_errorName(u_status));
  }
  return o_status;
}


