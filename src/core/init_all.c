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

#include <unicode/ustdio.h>
#include <unicode/ustring.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <assert.h>

#include "oly/common.h"
#include "oly/output.h"
#include "oly/error.h"
#include "oly/list.h"
#include "oly/syntax.h"
#include "oly/oly.h"


Oly_Status
init_all (Oly *oly, char *locale) {
  UErrorCode      u_status  = U_ZERO_ERROR; /* Unicode u_status code */
  clean_io_open();
  oly_result_clear (oly);
  
  init_io(locale);
  
  if ( cleanenv() != OLY_OKAY )
    oly_fatal (oly_result_get(oly));

  /* Initialize ICU */
  u_init(&u_status);
  if (U_FAILURE(u_status)) {
      oly_result_append (oly, "Can not initialize ICU.  u_status = ");
      oly_result_append (oly, u_errorName(u_status));
      oly_fatal (oly_result_get(oly));
  }
}


