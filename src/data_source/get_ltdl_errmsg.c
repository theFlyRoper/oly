/* get_ltdl_errmsg.c - LTDL error message function.  License GPL2+ {{{
 * Copyright (C) 2014 Oly Project
 * Permission is hereby granted, free of charge, to any person obtaining a copy of 
 * this software and associated documentation files (the "Software"), to deal in 
 * the Software without restriction, including without limitation the rights to 
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
 * of the Software, and to permit persons to whom the Software is furnished to do 
 * so, subject to the following conditions:
 * 
 *  The above copyright notice and this permission notice shall be included in 
 *  all copies or substantial portions of the Software.
 *  
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 * }}} */

#include "oly/common.h"
#include <ltdl.h>
#include "oly/core.h"
#include "pvt_core.h"

static const ResourceData * const get_ltdl_messages(void);

static const ResourceData * libltdl_message_data;

OChar *get_ltdl_errmsg( int status )
{
    int len = 0;
    OChar* result = NULL;

#ifdef HAVE_UNICODE_URES_H
    UErrorCode u_status = U_ZERO_ERROR;
    if (libltdl_message_data == NULL) 
    {
        libltdl_message_data = get_ltdl_messages();
    }

    if ((status < 0) 
            || (status >= LT_ERROR_MAX))
    {
        u_status = U_ZERO_ERROR;
        result = (OChar *)ures_getStringByIndex( libltdl_message_data, 
            (OLY_ERR_LTDL_UNKNOWN + OLY_STATUS_OFFSET),
            &len, &u_status );
    }
    else
    {
        result = (OChar *)ures_getStringByIndex( libltdl_message_data, 
                (status), &len, &u_status );
    }
    
    if (u_status == U_MISSING_RESOURCE_ERROR )
    {
        u_status = U_ZERO_ERROR;
        result = (OChar *)ures_getStringByIndex( libltdl_message_data, 
            (OLY_ERR_LTDL_UNKNOWN + OLY_STATUS_OFFSET),
            &len, &u_status );
    }
    else if (U_FAILURE(u_status))
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

static const ResourceData * const get_ltdl_messages(void)
{
#ifdef HAVE_UNICODE_URES_H
    UErrorCode u_status = U_ZERO_ERROR;
    ResourceData *retval = (ResourceData *)ures_getByKey(
            (UResourceBundle *)get_resource_data(oly->data), 
            "libltdlErrors", NULL, &u_status);
    if (U_FAILURE(u_status)) 
    {
        printf("Error messages not found! Status: %s.\n",
                u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    return retval;
#endif /* HAVE_UNICODE_URES_H */
}

