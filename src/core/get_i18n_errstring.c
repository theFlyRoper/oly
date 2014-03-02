/* get_i18n_errstring.c - get the ochar * error string for formatting. License GPL2+ {{{
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

#include <assert.h>

#include "oly/core.h"

static oly_messages *init_i18n_errors(void);

static oly_messages *
init_i18n_errors(void){
#ifdef HAVE_UNICODE_URES_H
    UResourceBundle *error_resource;
    UErrorCode      u_status  = U_ZERO_ERROR; 
    error_resource = ures_getByKey (OlyResources, "OlyErrors", 
                error_resource, &u_status);
    if (U_FAILURE(u_status)) {
        printf("Could not open error resources. status: %s. Exiting...\n", u_errorName(u_status));
        exit (EXIT_FAILURE);
    } else {
        return (oly_messages *)error_resource;
    }
#endif
}
const static oly_messages* oly_errors;

oly_status
get_i18n_errstring(ochar **message, int32_t *message_len,
        const oly_status oly_errno)
{
    int32_t result_len = 0;
    oly_status return_status = OLY_OKAY;
    oly_errors = init_i18n_errors();
#ifdef HAVE_UNICODE_URES_H
    UErrorCode u_status = U_ZERO_ERROR;
    ochar *err_text = NULL;
    assert(message_len != NULL);
    result_len = *message_len;
    
#ifdef OLYDEV
    list_package_locales("OlyErrors");
#endif /* OLYDEV */
    err_text = ures_getStringByIndex(
            oly_errors, oly_errno, &result_len, &u_status);

    if (U_FAILURE(u_status)) {
#ifdef OLYDEV
        fprintf(stderr, "oly error number %i not found. status: %s.\n", 
                oly_errno, u_errorName(u_status));
#endif /* OLYDEV */
        err_text = NULL;
        return_status = OLY_WARN_ERROR_NOT_FOUND;
    } else {
        *message = err_text;
    }
#endif
    return return_status;
}
