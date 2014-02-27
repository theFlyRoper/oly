/* oget_user_locale test License GPL2+ {{{
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

#include <stdio.h>
#include <assert.h>

#include "oly/common.h"
#include "oly/core.h"

int
main( void ){
  char          *locale;
  u_setDataDirectory(TEST_LOCALEDIR);
  
  /* oget_user_locale should always return SOME value.
   * in ICU, the default passed back when absolutely no 
   * language value is available on a Unix system is en_US_POSIX,
   * which is a good default.
   */
  assert((locale = oget_user_locale()) != NULL);
  
  printf("Locale: %s\n", locale);
  return EXIT_SUCCESS;
}


