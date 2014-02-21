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
#include "oly/loader.h"

char *
oget_user_locale (void) {
  /* currently follows GNU coding standards by environment variables:
   * LANGUAGE (a colon-separated list of language codes)
   * LC_ALL
   * LANG 
   */
  size_t i = 0, len = 1, langlen = 0;
  UAcceptResult acceptable;
  UErrorCode u_status = U_ZERO_ERROR;
  char    *language_val = getenv("LANGUAGE");
  char    sep[1] = ":";
  char    **curr = NULL, *locale = NULL, *word = NULL, *item = NULL, *ptr = NULL;
  printf("We're in the oget_user_locale function!\n");
  if (language_val != NULL) {
    UEnumeration *available = ures_openAvailableLocales(OLY_RESOURCE, &u_status);
    printf("We've just gotten available locales, yay! Language_val: %s\n", language_val);
    langlen = strlen(language_val);
    for (item = language_val; i<langlen; item++) {
      if ( *item == ':') {
        len++;
      } 
      i++;
    }
    
    i -= (len-1);
    printf("i minus len: %i\n",i);
    i += (len * sizeof(char *));
    printf("did some stuff. i = %i, len = %i.\n",i, len);
    if (!(curr = (char **)xmalloc(i))) {
      abort();
    }
    printf("Some other stuff.\n");

    curr[len - 1] = 0;
    i = 0;
    printf("Even more other stuff.\n");
    ptr = (char *)curr + (len * sizeof(char *));
    for (word = strtok_r(language_val, sep, &item); word; 
        word = strtok_r(NULL, sep, &item))
    {
      curr[i++] = ptr;
      len = strlen(word);
      memcpy(ptr, word, len + 1);
      ptr += len + 1;
      printf("curr[%i] == %s. word: %s.\n", i, ptr, word);
    }
    uloc_acceptLanguage(locale, len, &acceptable, (const char **)curr, 
        len, available, &u_status) ;
  } else {
    printf("Language is null.\n");
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

