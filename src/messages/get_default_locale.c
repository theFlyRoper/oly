/* get_default_locale.c - Oly function to retrieve user locale. License GPL2+ {{{
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

#include <unicode/ustdio.h>
#include <unicode/uloc.h>
#include <unicode/ures.h>

#include <sys/types.h>
#include <unistd.h>
#include <assert.h>

#include "oly/state.h"
#include "oly/core.h"

/* these used to be public but enh, why?  Not going to use them
 * elsewhere. 
 */

static int32_t count_tokens (char *s, char *delims);
static int32_t count_nondelim_chars (char *s, char *delims);
static char **token_str_to_array(char *s, char *delims, oly_status *status); 

oly_status get_default_locale (char *locale[], oly_status *status) 
{
    /* sorta follows the GNU coding standards; looks at LANGUAGE 
    * (a colon-separated list of language codes) first, then uses
    * ICU's uloc_getName function, which sets locale to the default 
    * if it is null.
    */
    int32_t         output_size = 0, len = 0;
    UAcceptResult   acceptable;
    UErrorCode      u_status  = U_ZERO_ERROR;
    char           *language_val = getenv("LANGUAGE"), sep[1] = ":";
    char          **curr = NULL, result[OLY_SMALL_BUFFER];
    *status             = OLY_OKAY;
    UEnumeration    *available;
    available       = ures_openAvailableLocales(OLY_RESOURCE, &u_status);
    while ( language_val != NULL ) 
    {
        if (U_FAILURE(u_status)) 
        {
            *status = OLY_ERR_LIB;
            break;
        }
        curr  = token_str_to_array(language_val, sep, status);
        len   = count_tokens (language_val, sep);
        output_size = uloc_acceptLanguage(result, OLY_SMALL_BUFFER, &acceptable, 
            (const char **)curr, len, available, &u_status) ;
        language_val = NULL;
        if (U_FAILURE(u_status)) 
        {
            printf("uloc_acceptLanguage failed: %s\n",  u_errorName(u_status));
            *status = OLY_ERR_LIB;
        }
    }
    if ((output_size <= 0) && (*status == OLY_OKAY))
    {
        output_size = uloc_getBaseName(NULL, result, OLY_SMALL_BUFFER, &u_status);
    }
    if (U_FAILURE(u_status)) 
    {
        printf("uloc_getBaseName failed: %s\n",  u_errorName(u_status));
        *status = OLY_ERR_LIB;
    }
    if (( output_size > 0 ) &&  ( *status == OLY_OKAY ))
    {
        *locale = (char *)xmalloc( (output_size + 1) * ( sizeof(char) ));
        strncpy(*locale, result, output_size);
        *((*locale) + output_size) = '\0';
    } 
    else if (*status == OLY_OKAY) 
    {
            *status = OLY_ERR_INIT;
    }
    return *status;
}


/* allocates a char ** array and copies each token into it. 
 * will always return a (char **) where there is at least one string with 
 * an empty value.
 */
char **
token_str_to_array(char *s, char *delims, oly_status *status) 
{
  const int32_t    arr_size = count_tokens(s,delims), 
                  num_chars = count_nondelim_chars(s,delims);
  int32_t          arr_ptr = 0, strsize = 0;
  /* we need arr_size char pointers and arr_size (for the EOLs) + num_chars chars. */
  char          **result = (char **)xmalloc(
                  ((arr_size * sizeof(char *)) 
                   + ((arr_size + num_chars) * sizeof(char)))),
                 *next, *result_position;

  assert(result != NULL);

  *status = OLY_OKAY;
  result_position = (char *)(result + (arr_size * sizeof(char *)));
  if ( delims != NULL ) {
    while ( s != NULL && *(s += strspn(s, delims)) != '\0' ) {
      next = strpbrk(s, delims);
      if (next != NULL) {
        strsize = (next - s);
      } else {
        strsize = strlen(s);
      };
      strncpy(result_position, s, strsize);
      *(result_position + strsize) = '\0';
      result[arr_ptr++] = result_position;
      result_position += (strsize +1);
      s = next;
    }
  }
  return result;
}

int32_t
count_tokens (char *s, char *delims)
{
  int32_t  current_count = 0;

  /* loop over s, adding 1 to current_count for each iteration. */
  if ( delims != NULL ) {
    while ( s != NULL && *(s += strspn(s, delims)) != '\0' ) {
      current_count++;
      s = strpbrk(s, delims);
    }
  }
  return current_count;
}

int32_t
count_nondelim_chars (char *s, char *delims)
{
  int32_t  current_count = 0;
  char    *next;

  /* delims should never be null. */
  assert( delims != NULL ) ;
  /* loop over s, adding the difference between the next and the primary */
  while ( s != NULL && *(s += strspn(s, delims)) != '\0' ) {
    next = strpbrk(s, delims);
    if ( next != NULL ) {
      current_count += ( next - s );
    } else {
      current_count += strlen(s);
    }
    s = next;
  }
  return current_count;
}


