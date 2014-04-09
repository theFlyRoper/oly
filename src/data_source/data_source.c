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
#include "data_source/boundary.h"
#include "pvt_data_source.h"
#include "oly/oly_dev.h"
#include "oly/core.h"


OlyStatus get_data_source_status( OlyDataSource *ds )
{
    if (ds == NULL)
    {
        return OLY_ERR_NO_OBJECT;
    }
    else
    { 
        return ds->status;
    }
}

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
run_ds_dispatch_function( OlyDataSource *ds )
{
    OlyStatus status = OLY_OKAY;
    status = (*ds->dispatch)(ds);
    return status;
}

OlyStatus 
set_ds_dispatch_function( OlyDataSource *ds, OlyDispatch function)
{
    OlyStatus status = OLY_OKAY;
    ds->dispatch = function;
    return status;
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

/* to simplify maintenance of the data source options besides locale, encoding and direction, we call set_data_option */

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
        HANDLE_OLY_STATUS(*status, return NULL);
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
    retval->encoding = NULL;
    retval->key_staging = ocalloc((max_key_size), sizeof(char));
    retval->key_stage_max_length = (max_key_size-1);
    retval->max_buffer_size = DEFAULT_BUFFER_SIZE;
    retval->external_scalar = ocalloc((max_key_size), sizeof(char));
    retval->internal_scalar = ocalloc((max_key_size), sizeof(OChar));
    retval->boundary = NULL;
    retval->sequence = 0;
    *status = new_oly_node(&(retval->node));
    retval->data = NULL;

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

/* for the sake of a consistent interface, encoding and locale return as OlyStatus 
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
set_data_encoding( OlyDataSource *ds, const char *encoding )
{
    ds->encoding = (char *)encoding;
    return OLY_OKAY;
}

char *
get_data_locale( OlyDataSource *ds )
{
    return ds->locale;
}

char *
get_data_encoding( OlyDataSource *ds )
{
    return ds->encoding;
}

OlyStatus set_data_source_direction (OlyDataSource *ds, OlyDSDirection direction)
{
    ds->direction = direction ;
    return OLY_OKAY;
}

OlyStatus get_data_source_direction (OlyDataSource *ds, OlyDSDirection *direction)
{
    *direction = ds->direction;
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
    if ( ds->encoding != NULL )
    {
        free_me = (void *)ds->encoding;
        OFREE(free_me);
    }
    if ( ds->boundary != NULL )
    {
        close_oly_boundary(ds->boundary);
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
stage_node_key( OlyDataSource *ds, const char *key, size_t key_len )
{
    if (key_len == 0)
    {
        key_len = strlen(key);
    }
    if ( ds->status != OLY_OKAY )
    {
        HANDLE_STATUS_AND_RETURN(ds->status);
        return ds->status;
    }
    if ( key == NULL )
    {
        ds->status = OLY_ERR_NO_KEY_BUFFER;
    }
    if ( key_len >= ds->key_stage_max_length )
    {
        ds->status = OLY_ERR_KEY_STR_TOO_LONG;
    }
    strncpy(ds->key_staging, key, (key_len +1));
    return ds->status;
}

/* enqueue_ds_node resets the key along with adding the node to the queue */
OlyStatus 
enqueue_ds_node( OlyDataSource *ds, void *value, OlyNodeValueType type)
{
    size_t           buffer_needed_value = 0, buffer_needed_key = 0, length = 0;
    OlyNode         *new_node = NULL; /* *curr_node = get_current_node(ds); */
    bool             has_string = false;
    /* unsigned short   depth; */
    HANDLE_STATUS_AND_RETURN(ds->status);
    /* depth = get_ */
    new_node = ds->node;
    reset_node(new_node);
    ds->status = set_node_tuple( new_node, (ds->sequence++) );
    HANDLE_STATUS_AND_RETURN(ds->status);
    
    /* if key *value != \0, append it to the converter buffer and set *key to \0. */
    if ( ( *(ds->key_staging) != '\0' ) && ( ds->status == OLY_OKAY ) )
    {
        set_node_has_key(new_node);
        has_string = true;
        buffer_needed_key += strlen( ds->key_staging );
        ds->status = put_char_in( ds->boundary , (const char *)ds->key_staging,
                buffer_needed_key );
    }

    switch (type)
    {
        case OLY_NODE_VALUE_SCALAR_STRING:
            /* if value is str, check length. */
            buffer_needed_value += (strlen( (char *)value ) + 1);
            ds->status = put_char_in( ds->boundary , (const char *)value ,
                    buffer_needed_value );
            break;
        case OLY_NODE_VALUE_TYPE_MAP:
        case OLY_NODE_VALUE_TYPE_SEQUENCE:
            ds->status = descend_one_level(ds->node);
        default:
            break;
    }
    ds->status = set_node_value( new_node, value, type );
    HANDLE_STATUS_AND_RETURN(ds->status);
   
    if ( has_string == true )
    {
        ds->status = flush_inbound( ds->boundary );
    }

    if  (*(ds->key_staging) != '\0')
    {
        *(ds->key_staging) = '\0';
        ds->status = get_ochar_out( ds->boundary, &(ds->internal_scalar), &length);
        if (ds->status != OLY_WARN_BOUNDARY_RESET)
        {
            ds->status = OLY_OKAY;
        }
        HANDLE_STATUS_AND_RETURN(ds->status);
        ds->status = set_node_key(ds->node, (const OChar *)ds->internal_scalar);
        HANDLE_STATUS_AND_RETURN(ds->status);
    }
    else
    {
        set_node_key(ds->node, NULL);
    }

    if (OLY_NODE_VALUE_SCALAR_STRING == type)
    {
        ds->status = get_ochar_out( ds->boundary, &(ds->internal_scalar), &length);
        if (ds->status != OLY_WARN_BOUNDARY_RESET)
        {
            ds->status = OLY_OKAY;
        }
        HANDLE_STATUS_AND_RETURN(ds->status);
        ds->status = set_node_string_value(ds->node, (const OChar *)ds->internal_scalar);
        HANDLE_STATUS_AND_RETURN(ds->status);
    }
    ds->status = OLY_WARN_NODE_PRODUCED;
    
    return ds->status;
}

void
reset_ds_status ( OlyDataSource *ds )
{
    ds->status = OLY_OKAY;
}

OlyStatus 
get_boundary( OlyDataSource *ds, OlyBoundary **bound )
{
    ds->status = OLY_OKAY;
    (*bound) = ds->boundary;
    return ds->status;
}

/* Used by oly internal to collect and dispense nodes to and from its internal queue. */
OlyStatus collect_nodes( OlyDataSource *ds, OlyNode **node_out)
{
    OlyStatus status = OLY_OKAY;
    (*node_out) = ds->node;
    return status;
}

/* TODO: flesh these out and turn them in to proper dequeues! */
OlyStatus dispense_nodes( OlyDataSource *ds, OlyNode *node_in)
{
    OlyStatus status = OLY_OKAY;
    ds->node = node_in;
    return status;
}


/* enqueue and dequeue are for external interfaces. */

OlyStatus dequeue_ds_node( OlyDataSource *ds, OlyNode **node )
{
    OChar *key;
    (*node) = ds->node;
    get_node_key(*node, &key) ;
    return ds->status;
}

OlyStatus open_ds_boundary( OlyDataSource *ds )
{
    ds->status = open_oly_boundary(ds->encoding, ds->max_buffer_size,
            &(ds->boundary));
    return ds->status;
}

OlyStatus set_ds_data( OlyDataSource *ds, void *data )
{
    OlyStatus status = OLY_OKAY;
    if (data == NULL)
    {
        status = OLY_ERR_NO_OBJECT;
    }
    ds->data = data;
    return status;
}

void *get_ds_data( OlyDataSource *ds )
{
    return ds->data;
}

OlyStatus 
ds_descend( OlyDataSource *ds )
{
    OlyStatus  status = OLY_OKAY;
    ds->status = descend_one_level(ds->node);
    return status;
}

OlyStatus
ds_ascend( OlyDataSource *ds )
{
    OlyStatus  status = OLY_OKAY;
    status = ascend_one_level(ds->node);
    return status;
}

