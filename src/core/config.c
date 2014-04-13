/* config.c - load and parse configuration.  License GPL 2+{{{
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
#include "oly/olytypes.h"
#include <stdbool.h>
#include "pvt_config.h"

/* the error handler stays separate to minimize the risk of errors
 * within it.  Also, no other part of Oly needs access to the config
 * data.
 */
static const ResourceData * const init_main_config(void);
static const ResourceData * config_data;

OlyStatus get_config_item( OChar **result, OChar *key )
{
    OlyStatus status = OLY_OKAY;
    int len = 0;
#ifdef HAVE_UNICODE_URES_H
    UErrorCode u_status = U_ZERO_ERROR;
    *result = get_errmsg(OLY_ERR_UNKNOWN);
    if (u_status == U_MISSING_RESOURCE_ERROR )
    {
        u_status = U_ZERO_ERROR;
        *result = (OChar *)ures_getStringByIndex( config_data, 
            (OLY_ERR_UNKNOWN + OLY_STATUS_OFFSET),
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
    return status; 
}

OlyStatus init_config(Oly *oly)
{
    OlyStatus status = OLY_OKAY;
    if (config_data == NULL) 
    {
        config_data = init_main_config();
    }
    else
    {
        status = OLY_WARN_REINIT;
    }
    return status;
}

static const ResourceData * const init_main_config(void)
{
#ifdef HAVE_UNICODE_URES_H
    UErrorCode u_status = U_ZERO_ERROR;
    ResourceData *retval = NULL;
    UResourceBundle *firstval = ures_getByKey((UResourceBundle *)get_oly_resource(oly), 
            "OlyConfig", NULL, &u_status);
    if (U_FAILURE(u_status)) 
    {
        printf("Error configs not found! Status: %s.\n",
                u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    retval = (ResourceData *)ures_getByKey(firstval,
            "OlyMain", NULL, &u_status);
    if (U_FAILURE(u_status)) 
    {
        printf("Error configs not found! Status: %s.\n",
                u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    return retval;
#endif /* HAVE_UNICODE_URES_H */
}

/*
void *get_config_item(OlyConfigItem record)
{
    size_t   size_record;
    void    *item
    switch (record)
    {
        default:
            size_record =(size_t)(BUFSIZ*4); 
    }
    item = (void *)&size_record;
    return item;
}
*/

