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

size_t 
get_max_buffer_size(OlyDataSource *ds, OlyStatus *status)
{
    size_t mbuff = 0;

    if (ds->max_buffer_size == 0)
    {
        *status = OLY_WARN_DS_BUFFER_DEFAULT;
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

    OlyStatus status = OLY_OKAY;
    if (mbuff == 0)
    {
        status = OLY_WARN_DS_BUFFER_DEFAULT;
        ds->max_buffer_size = DEFAULT_BUFFER_SIZE;
    }
    else
    {
        ds->max_buffer_size = mbuff;
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

char *get_data_locale( OlyDataSource *ds )
{
    return ds->locale;
}

char *get_data_charset( OlyDataSource *ds )
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

OlyBoundary *
open_oly_boundary(OlyDataSource *ds, OlyStatus *status)
{
    UErrorCode   u_status = U_ZERO_ERROR;
    size_t       o_size = 0, c_size = 0, max_characters = 0, min_char_size = 0,
                 buffer_max_size = get_max_buffer_size(ds, status);
    char        *break_ptr;
    OlyBoundary *oly_bound = omalloc(sizeof(OlyBoundary));
    /* oly boundary uses two buffers:
     * 1. Buffer full of input characters
     * 2. Buffer full of OChars 
     *
     * to use it, drop chars in the input buffer and once it is full it will flush
     * them.
     */

    /* Allocate the space for both buffers,
     * then chop it into two spaces according to character size ratios. 
     * Add sizeof(OChar) to accomodate an OChar sized null space to 
     * delineate the two areas. 
     * Consertative and wastes a bit of the end of the buffer.
     * Better that then overflows.
     */
    oly_bound->converter   = ucnv_open( get_data_charset(ds), &u_status );
    min_char_size = ucnv_getMinCharSize(oly_bound->converter) ;
    max_characters      = (buffer_max_size / ( min_char_size + sizeof(OChar)));
    o_size          = (( max_characters * sizeof(OChar) ) - sizeof(OChar));
    c_size          = (( max_characters * min_char_size ) - min_char_size);

    /* normally the ICU converter API only uses 6 pointers, 
     * 3 for the outside charset (c_) and 3 for the ICU internal
     * UChar structures (o_).  The flush_break pointers are 
     * there to hold the spot for higher nodes. 
     * 
     * because the char buffers are actually pointing to different 
     * character sets, although we only expect to find max_characters characters in it,
     * as they are char pointers we must move it the correct number of chars. */
    oly_bound->c_now = (char *)omalloc( o_size +c_size +min_char_size +sizeof(OChar));
    oly_bound->c_start          = oly_bound->c_now;
    oly_bound->c_end            = ((oly_bound->c_now + 
                ((max_characters*min_char_size)- min_char_size*2)));
    oly_bound->c_flush_break    = oly_bound->c_now;

    /* OChar buffer
     * unlike the char buffer, the OChar buffer points to its actual character set.
     * Thus, it must only move max_characters forward, as opposed to 
     * o_size. */
    oly_bound->o_now            = (OChar *)((oly_bound->c_end) + min_char_size + 1);
    oly_bound->o_start          = oly_bound->o_now;
    oly_bound->o_end            = ((oly_bound->o_now) + (max_characters-2));
    oly_bound->o_flush_break    = oly_bound->o_now;
    
    /* make sure the area after each buffer is null. */
    for ( break_ptr = ((oly_bound->c_end) + 1); 
            ( break_ptr < (char *)oly_bound->o_now ); break_ptr++ )
    {
        *break_ptr = '\0' ;
    }
    
    for ( break_ptr = (char *)(oly_bound->o_end); 
            ( break_ptr < (oly_bound->c_start + buffer_max_size )); break_ptr++ )
    {
        *break_ptr = '\0' ;
    }
   
    /* assert same number of characters fit in both buffers. */
    o_size = (size_t)(oly_bound->o_end - oly_bound->o_start);
    c_size = (size_t)((oly_bound->c_end - oly_bound->c_start)/min_char_size);
    assert( o_size == c_size );

    /* assert total sum of size is less than max size. */
    o_size = (size_t)((char *)oly_bound->o_end - (char *)oly_bound->o_start);
    c_size = (size_t)(oly_bound->c_end - oly_bound->c_start);
    assert( ( o_size + c_size ) < buffer_max_size );
    /* o_size and c_size contain exact multiples of the size of their respective character sets. */
    assert( ( o_size % sizeof(OChar) ) == 0 );
    assert( ( c_size % min_char_size ) == 0 );

    return oly_bound;
};


