/* get_errmsg.c - Return an internationalized error message.  License GPL2+ {{{
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
#include "oly/core.h"
#include "pvt_core.h"
/* the error handler stays separate to minimize the risk of errors
 * within it.  Also, no other part of Oly needs access to the message
 * data.
 */
static const ResourceData * const init_messages(void);

static const ResourceData * message_data;
/* do not call before initializing main oly structure. 
 * This seems unlikely unless the
 * main oly structure has been destroyed and if that
 * happened, there are bigger problems.
 */
OChar *get_errmsg( OlyStatus status )
{
    int len = 0;
#ifdef HAVE_UNICODE_URES_H
    UErrorCode u_status = U_ZERO_ERROR;
    OChar* result = NULL;
    if (message_data == NULL) 
    {
        message_data = init_messages();
    }

    if ((status < OLY_STATUS_MIN) 
            || (status > OLY_STATUS_MAX))
    {
        u_status = U_ZERO_ERROR;
        result = (OChar *)ures_getStringByIndex( message_data, 
            (OLY_ERR_UNKNOWN + OLY_STATUS_OFFSET),
            &len, &u_status );
    }
    else
    {
        result = (OChar *)ures_getStringByIndex( message_data, 
                (status + OLY_STATUS_OFFSET), &len, &u_status );
    }
    
    if (U_FAILURE(u_status))
    {
        /* This will catch when we forget to update the list of errors in root.txt, 
         * or any other ICU error.
         */
        fprintf(stderr, "ICU Error: %s.\n",
                u_errorName(u_status));
    }
#endif /* HAVE_UNICODE_URES_H */
    return result; 
}

static const ResourceData * const init_messages(void)
{
#ifdef HAVE_UNICODE_URES_H
    UErrorCode u_status = U_ZERO_ERROR;
    ResourceData *retval = (ResourceData *)ures_getByKey(
            (UResourceBundle *)get_resource_data(oly->data), 
            "OlyErrors", NULL, &u_status);
    if (U_FAILURE(u_status)) 
    {
        printf("Error messages not found! Status: %s.\n",
                u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    return retval;
#endif /* HAVE_UNICODE_URES_H */
}

