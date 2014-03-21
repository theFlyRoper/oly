/* data_source_main.c - Oly data source interaction functions License GPL 2+{{{
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
#include <sys/types.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>

#include <yaml.h>

#include "oly/data_source.h"
#include "pvt_data_source.h"
#include "oly/oly_dev.h"
#include "oly/core.h"
/* set_datasource_function, get_data_interface, set_data_interface, 
OlyStatus
set_datasource_function( OlyDataSource *ds, 
        OlyDSFunctionType ds_func_type, DataSourceFunction *ds_function)
{
    OlyStatus status = OLY_OKAY;
    assert((ds_function != NULL) && (ds != NULL));
    switch(ds_func_type)
    { 
        case OLYDS_INIT_FUNCTION:
            ds->init_function = ds_function;
            break;
        case OLYDS_OPEN_FUNCTION:
            ds->open_function = ds_function;
            break;
        case OLYDS_DELETE_FUNCTION:
            ds->delete_function = ds_function;
            break;
        default:
            status = OLY_ERR_UNKNOWN_FUNCTION_TYPE;
            break;
    }
    return status;
}

OlyDataStream *get_data_interface( OlyDataSource *ds, OlyStatus *status)
{
    if (*status != OLY_OKAY)
    {
        return NULL;
    }
    else
    {
        return ds->data ; 
    }

}

OlyStatus set_data_interface( OlyDataSource *ds, OlyDataStream *interface)
{
    OlyStatus status = OLY_OKAY;
    ds->data = interface;
    return status;
}

*/

OlyStatus
set_ds_option_required( OlyDataSource *ds, DataSourceOptions option )
{
    OlyStatus status = OLY_OKAY;
    if ((option > (sizeof(long)*CHAR_BIT)) || 
            ( ds->unused_settings & (unsigned int)pow(2,option) ) == option )
    {
        status = OLY_ERR_DS_OPTION_CONFLICT;
    }
    else 
    {
        ds->required_settings |= option;
    }

    return status;
}

OlyStatus
set_ds_option_unused( OlyDataSource *ds, DataSourceOptions option )
{
    OlyStatus status = OLY_OKAY;
    if ((option > (sizeof(long)*CHAR_BIT)) || 
            ( ds->required_settings & (unsigned int)pow(2,option) ) == option )
    {
        status = OLY_ERR_DS_OPTION_CONFLICT;
    }
    else 
    {
        ds->unused_settings |= option;
    }

    return status;
}

/* to simplify maintenance of the data source options besides locale, charset and direction, we call set_data_option */

OlyStatus 
set_data_option( OlyDataSource *ds, const DataSourceOptions option, const char *value )
{
    OlyStatus status = OLY_OKAY;
    if ((ds->unused_settings & option) == option)
    {
        status = OLY_WARN_DSOPT_NOT_USED;
    }
    else
    {
        ds->options[option] = (char *)ostrdup(value);
    }
    return status;
}

char *
get_data_option( OlyDataSource *ds, const DataSourceOptions option, OlyStatus *status )
{
    char    *opt = NULL;
    if ((ds->unused_settings & option) == option)
    {
        *status = OLY_WARN_DSOPT_NOT_USED;
    }
    else
    {
        opt = ds->options[option];
    }
    return opt;
}

OlyDataSource *
new_data_source( DataSourceType ds_type, OlyStatus *status )
{
    unsigned int     i = 0;
    OlyDataSource   *retval;
    if (*status != OLY_OKAY)
    {
        return NULL;
    }

    if ((ds_type > DS_TYPE_MAX) || (ds_type < DS_TYPE_MIN))
    {
        *status = OLY_ERR_BADARG;
        return NULL;
    }
    retval = (OlyDataSource *)ocalloc(1, sizeof(OlyDataSource));
    for ( i = 0; ( i <= DSOPT_MAX ); i++ )
    {
        retval->options[i] = NULL;
    }

    retval->ds_type = ds_type;
    retval->direction = OLY_DS_IN;
    retval->unused_settings = 0x0;
    retval->required_settings = 0x0;
    retval->locale = NULL;
    retval->charset = NULL;
/*    retval->init_function = NULL;
    retval->open_function = NULL;
    retval->delete_function = NULL; 
    */
    return retval;
}

OlyStatus 
set_data_direction( OlyDataSource *ds, OlyDSDirection ds_io)
{
    ds->direction = ds_io;
    return OLY_OKAY;
}

/* for the sake of a consistent interface, charset and locale return as OlyStatus 
 * Options for later:
 *  Check them against ICU?
 *  Check them against the datasource? */
OlyStatus 
set_data_locale( OlyDataSource *ds, const char *locale )
{
    ds->locale = (char *)locale;
    return OLY_OKAY;
}

OlyStatus 
set_data_charset( OlyDataSource *ds, const char *charset )
{
    ds->charset = (char *)charset;
    return OLY_OKAY;
}

OlyStatus 
close_data_source( OlyDataSource *ds )
{
    int      i = 0;
    void    *free_me;
    if ( ds == NULL )
    {
        return OLY_OKAY;
    }
    for ( i = 0; ( i <= DSOPT_MAX ); i++ )
    {
        if ( ds->options[i] != NULL )
        {
            free_me = (void *)ds->options[i];
            OFREE(free_me);
        }
    }
    
    if ( ds->locale != NULL )
    {
        free_me = (void *)ds->locale;
        OFREE(free_me);
    }
    if ( ds->charset != NULL )
    {
        free_me = (void *)ds->charset;
        OFREE(free_me);
    }
    free_me = (void *)ds;
    OFREE(free_me);
    return OLY_OKAY;
}

OlyStatus 
set_data_filename( OlyDataSource *ds, const char *filename )
{
    ds->filename = (char *)filename;
    return OLY_OKAY;
}

