/* oget_user_locale.c - Oly function to retrieve user locale. License GPL2+ {{{
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

#include <unicode/ustdio.h>
#include <unicode/ures.h>

#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

#include "oly/error.h"
#include "oly/core.h"

char *
oget_user_locale (void) {
  /* sorta follows the GNU coding standards; looks at LANGUAGE 
   * (a colon-separated list of language codes) first, then uses
   * ICU's uloc_getName function, which sets locale to the default 
   * if it is null.
   */
  const size_t    buffer_size = OLY_SMALL_BUFFER;
  size_t          output_size = 0, len = 0;
  UAcceptResult   acceptable;
  UErrorCode      u_status  = U_ZERO_ERROR;
  Oly_Status      status    = OLY_OKAY;
  char           *language_val = getenv("LANGUAGE"), sep[1] = ":";
  char          **curr = NULL, result[buffer_size], *locale = NULL;
  
  if ( language_val != NULL ) {
    UEnumeration *available = ures_openAvailableLocales(OLY_RESOURCE, &u_status);
    curr  = token_str_to_array(language_val, sep, &status);
    len   = count_tokens (language_val, sep);
    output_size = (size_t)uloc_acceptLanguage(result, buffer_size, &acceptable, 
        (const char **)curr, len, available, &u_status) ;
  }

  if ( output_size == 0 ) {
    output_size = (size_t)uloc_getName( (const char *)locale, 
        result, buffer_size, &u_status );
  }

  if ( output_size > 0 ) {
    locale = (char *)xmalloc( (output_size + 1) * ( sizeof(char) ));
    strncpy(locale, result, output_size);
    *(locale + output_size + 1) = '\0';
  }

  return locale;
}

