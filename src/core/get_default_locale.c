/* get_default_locale.c - returns the best match locale for the user. License GPL2+ {{{
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

/* intended to be included in init_oly. */

OlyStatus get_default_locale (char *locale[],
                OlyStatus *status)
{
    /* sorta follows the GNU coding standards; looks at LANGUAGE 
     * (a colon-separated list of language codes) first, then uses
     * ICU's uloc_getName function, which sets locale to the default 
     * if it is null.
     */
    int32_t         output_size = 0, tokens = 0, characters = 0, pre_size = 0, i = 0;
    UAcceptResult   acceptable;
    UErrorCode      u_status  = U_ZERO_ERROR;
    char           *language_val = NULL, sep[1] = ":", 
                   *language_preflight = getenv("LANGUAGE"),
                  **curr = NULL, result[OLY_SMALL_BUFFER], *r;
    UEnumeration    *available;
    available       = ures_openAvailableLocales(OLY_RESOURCE, &u_status);
    *status         = OLY_OKAY;
    r = result;
    if (U_FAILURE(u_status)) 
    {
        printf("ures_openAvailableLocales failed: %s\n",  u_errorName(u_status));
        *status = OLY_ERR_LIB;
    }
    if (*locale != NULL)
    {
        /* +1 to make room for the colon. */
        pre_size += (strlen(*locale)+1);
    }
    if (language_preflight != NULL)
    {
        pre_size += strlen(language_preflight);
    }
    if (pre_size > 0)
    {
        language_val = (char *)xmalloc((pre_size+1)*sizeof(char));
        if (*locale != NULL)
        {
            strcpy(language_val, *locale);
            strcat(language_val, ":");
            printf("langval: %s\n", language_val);
        }
        if (language_preflight != NULL)
        {
            strcat(language_val, language_preflight);
        }
    }
    
    while ( language_val != NULL )
    {
        printf("indaloooop: %s\n", language_val);
        if (U_FAILURE(u_status)) 
        {
            *status = OLY_ERR_LIB;
            break;
        }
        curr  = token_str_to_array(language_val, sep, &characters, &tokens, status);
        printf("toke it up yo: %s, chars: %i, tokens: %i\n", curr[1], characters, tokens);
        for (i = 0; (i<tokens); i++)
        {
            printf("curr[%i] = %s\n", i, curr[i]);
        }

        output_size = uloc_acceptLanguage(&result, OLY_SMALL_BUFFER, &acceptable, 
            (const char **)curr, tokens, available, &u_status) ;
        printf("re salt: %s\n", result);
        language_val = NULL;
        if (U_FAILURE(u_status)) 
        {
            printf("uloc_acceptLanguage failed: %s\n",  u_errorName(u_status));
            *status = OLY_ERR_LIB;
        }
    }
    uenum_close(available);
    if ((output_size <= 0) && (*status == OLY_OKAY))
    {
        output_size = uloc_getBaseName(NULL, r, OLY_SMALL_BUFFER, &u_status);
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
