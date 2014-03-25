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

static OlyStatus flush_data_source( OlyDataSource *ds );

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

OlyStatus 
set_ds_dispatch_function( OlyDataSource *ds, OlyDispatch function)
{
    return OLY_OKAY;
}

size_t 
get_max_buffer_size(OlyDataSource *ds)
{
    size_t mbuff = 0;

    if (ds->max_buffer_size == 0)
    {
        ds->status = OLY_WARN_DS_BUFFER_DEFAULT;
        mbuff = DEFAULT_BUFFER_SIZE;
    }
    else
    {
        mbuff = ds->max_buffer_size;
    }
    return mbuff;
}

OlyStatus
set_max_buffer_size(OlyDataSource *ds, size_t mbuff)
{

    if (mbuff == 0)
    {

        ds->status = OLY_WARN_DS_BUFFER_DEFAULT;
        ds->max_buffer_size = DEFAULT_BUFFER_SIZE;
    }
    else
    {
        ds->max_buffer_size = mbuff;
    }
    return ds->status;
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
    size_t           max_key_size = MAX_KEY_LENGTH;
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
    retval->key_staging = ocalloc((max_key_size), 1);
    retval->key_stage_max_length = (max_key_size-1);
    retval->node_list_size = (size_t)MAX_NODE_DEPTH;
    retval->node_count_now = (size_t)0;
    retval->node_list = (OlyNode **)ocalloc(retval->node_list_size, sizeof(OlyNode *));

/*  retval->init_function = NULL;
    retval->open_function = NULL;
    retval->delete_function = NULL; */
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

char *
get_data_locale( OlyDataSource *ds )
{
    return ds->locale;
}

char *
get_data_charset( OlyDataSource *ds )
{
    return ds->charset;
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

OlyStatus
stage_node_key( OlyDataSource *ds, const char *key )
{
    size_t      key_len = 0;
    if ( ds->status != OLY_OKAY )
    {
        return ds->status;
    }
    if ( key == NULL )
    {
        ds->status = OLY_ERR_NO_KEY_BUFFER;
    }
    key_len = ( strlen(key) + 1 );
    if ( key_len >= ds->key_stage_max_length )
    {
        ds->status = OLY_ERR_KEY_STR_TOO_LONG;
    }
    strncpy(ds->key_staging, key, ds->key_stage_max_length);
    return ds->status;
}

OlyStatus
handle_ds_status( OlyDataSource *ds )
{
    u_fprintf_u(u_stderr, get_errmsg(ds->status));
    return OLY_OKAY;
}

/* enqueue_ds_node resets the key along with adding the node to the queue */
OlyStatus 
enqueue_ds_node( OlyDataSource *ds, void *value, OlyNodeValueType type)
{
    size_t       buffer_needed_value = 0, buffer_needed_key = 0;
    OlyNode     *new_node = NULL;
    
    if ( ds->status != OLY_OKAY )
    {
        return ds->status;
    }

    /* node_count_now must increase, and if it is too big, flush the node list */
    if ( (ds->node_count_now++) >= ds->node_list_size )
    {
        ds->status = flush_data_source(ds);
        if ( ds->status != OLY_OKAY )
        {
            return ds->status;
        }
    }

    new_node = (ds->node_list)[(ds->node_count_now)];
    ds->status = set_node_tuple( new_node, (ds->sequence++) );
    if ( ds->status != OLY_OKAY )
    {
        return ds->status;
    }
    ds->status = set_node_value( new_node, value, type );
    
    if ( ds->status != OLY_OKAY )
    {
        return ds->status;
    }
    
    /* if value is str, check length. */
    if (( type == OLY_NODE_VALUE_SCALAR_STRING ) && ( ds->status == OLY_OKAY ))
    {
        buffer_needed_value += (strlen( (char *)value ) + 1);
    }
    /* if key *value != \0, append it to the converter buffer and set *key to \0. */
    if ( ( *(ds->key_staging) != '\0' ) && ( ds->status == OLY_OKAY ) )
    {
        buffer_needed_key += (strlen( ds->key_staging ) + 1);
    }
    return ds->status;
}

OlyStatus flush_data_source( OlyDataSource *ds )
{
    size_t i = 0;
    if ( ds->status != OLY_OKAY )
    {
        return ds->status;
    }
    
    ds->status = flush_inbound( ds->buffer );
    if ( ds->status != OLY_OKAY )
    {
        return ds->status;
    }
    
    ds->status = copy_ochar_buffer( ds->buffer, ds->destination->buffer );
    if ( ds->status != OLY_OKAY )
    {
        return ds->status;
    }

    for ( i = 0; ( i < ds->node_count_now ); i++ )
    {
        copy_node( ds->node_list[i], ds->destination->node_list[i] );
        reset_node( ds->node_list[i] );
    }
    
    ds->node_count_now = 0;
    ds->status = (*ds->destination->dispatch)(ds->destination);
    return ds->status;
}

extern OlyStatus dequeue_ds_node( OlyDataSource *ds, OlyNode *node );


