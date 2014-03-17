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
#include "sys/types.h"
#include "stdbool.h"

#include <yaml.h>

#include "data_source/oly_yaml.h"
#include "oly/data_source.h"
#include "pvt_data_source.h"
#include "oly/oly_dev.h"
#include "oly/core.h"

static DataSourceFormat get_data_source_format(const DataSourceType dst);

OlyDataSource *
new_data_source( DataSourceType ds_type, OlyStatus *status )
{
    OlyDataSource *retval;
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

    retval->dstype = dst;
    retval->direction = IN;
    retval->dsformat = get_data_source_format(dst);
    retval->locale = NULL;
    retval->charset = NULL;
    retval->filename = NULL;
    retval->username = NULL;
    retval->connect_string = NULL;
    retval->port = 0;
    retval->data = NULL;
}

OlyStatus 
set_data_filename( OlyDataSource *ds, const char *filename )
{
    ds->filename = (char *)filename;
    return OLY_OKAY;
}

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
set_data_connection_string( OlyDataSource *ds, const char *connection_string )
{
    ds->connection_string = (char *)connection_string;
    return OLY_OKAY;
}

OlyStatus 
close_data_source( OlyDataSource *ds )
{
    void *free_me;
    if ( ds == NULL )
    {
        return OLY_OKAY;
    }
    if ( ds->connection_string != NULL )
    {
        free_me = (void *)ds->connection_string;
        ofree(free_me);
    }
    
    if ( ds->locale != NULL )
    {
        free_me = (void *)ds->locale;
        ofree(free_me);
    }
    if ( ds->charset != NULL )
    {
        free_me = (void *)ds->charset;
        ofree(free_me);
    }
    if ( ds->filename != NULL )
    {
        free_me = (void *)ds->filename;
        ofree(free_me);
    }
    if ( ds->username != NULL )
    {
        free_me = (void *)ds->username;
        ofree(free_me);
    }
    if ( ds->connect_string != NULL )
    {
        free_me = (void *)ds->connect_string;
        ofree(free_me);
    }
    if ( ds->data != NULL )
    {
        free_me = (void *)ds->data;
        ofree(free_me);
    }
    free_me = (void *)ds;
    ofree(free_me);
    return OLY_OKAY;
}

static DataSourceFormat 
get_data_source_format(const DataSourceType dst)
{
    switch(dst)
    { 
        case YAML_FILE :
            return DATA_IN_FLAT_FILE;
        default :
            return UNKNOWN;
    }

}

