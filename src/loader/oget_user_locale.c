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
#include "common.h"

#include <unicode/ustdio.h>
#include <unicode/ures.h>

#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

#include "error.h"
#include "loader.h"

char *
oget_user_locale (void) {
  /* currently follows GNU coding standards by environment variables:
   * LANGUAGE (a colon-separated list of language codes)
   * LC_ALL
   * LANG 
   */
  int32_t i = 0;
  int32_t len = 0;
  UAcceptResult acceptable;
  UErrorCode *u_status = U_ZERO_ERROR;
  char    *language_val = getenv("LANGUAGE");
  char    *locale = NULL;
  const char  **curr = NULL;
  char    *item = NULL;
  if (language_val != NULL) {
    UEnumeration *available = ures_openAvailableLocales("oly_lang", u_status);
    curr[i] = strtok_r(language_val, ":", &item);
    for (i = 1; ((curr[i] = strdup(strtok_r(NULL, ":", &item))) != NULL); i++)
      ;
    curr[i] = NULL;
    uloc_acceptLanguage(locale, len, &acceptable, curr, i, available, u_status) ;
  }

  if (locale == NULL) {
    locale = xstrdup(getenv("LC_ALL"));
  }

  if (locale == NULL) {
    locale = xstrdup(getenv("LANG"));
  }
  
  if (locale == NULL) {
    locale = xstrdup("root");
  }

  return locale;
}

