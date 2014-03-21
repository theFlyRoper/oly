/* data_source.c - Oly data source dynamic loader License GPL 2+{{{
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

#include <ltdl.h>

#define OLY_DATA_SOURCE_DIR ".olyrc/data_source"
#define DIR_SEPARATOR "/"

OlyDataSource * 
load_ds (const char *name, OlyStatus *status)
{
    lt_dlhandle     ds;
    unsigned int    ltdl_status = 0;
    char            char_err = NULL;

    last_error = NULL;

    module = lt_dlopenext (name);

    if (module)
    {
        builtin_table = (Builtin*) lt_dlsym (module, "builtin_table");
        syntax_table = (Syntax *) lt_dlsym (module, "syntax_table");
        if (!builtin_table && !syntax_table)
            {
            lt_dlclose (module);
            last_error = no_builtin_table_error;
            module = NULL;
            }
    }

    if (module)
    {
        ModuleInit *init_func
            = (ModuleInit *) lt_dlsym (module, "module_init");
        if (init_func)
            (*init_func) (sic);
    }

    if (module)
        {
        SyntaxFinish *syntax_finish
            = (SyntaxFinish *) lt_dlsym (module, "syntax_finish");
        SyntaxInit *syntax_init
            = (SyntaxInit *) lt_dlsym (module, "syntax_init");

        if (syntax_finish)
            sic->syntax_finish = list_cons (list_new (syntax_finish),
                                            sic->syntax_finish);
        if (syntax_init)
            sic->syntax_init = list_cons (list_new (syntax_init),
                                        sic->syntax_init);
        }

    if (module)
        {
        if (builtin_table)
            status = builtin_install (sic, builtin_table);

        if (syntax_table && status == SIC_OKAY)
            status = syntax_install (sic, module, syntax_table);

        return status;
        }

    last_error = lt_dlerror();
    if (!last_error)
        last_error = module_not_found_error;

    return SIC_ERROR;
}

int
data_source_init (void)
{
    static int initialised = 0;
    int errors = 0;

    /* Only perform the initialisation once. */
    if (!initialised)
    {
        /* ltdl should use the same mallocation as us. */
        lt_dlmalloc = (lt_ptr_t (*) (size_t)) omalloc;
        lt_dlfree = (void (*) (lt_ptr_t)) ofree;

        /* Make sure preloaded data_sources are initialised. */
        LTDL_SET_PRELOADED_SYMBOLS();

        last_error = NULL;
        errors = lt_dlinit();
        /* Set up the data_source search directories. */
        if (errors == 0)
        {
            const char *path = ocalloc((strlen(getenv ("HOME"))+1)
                    + (strlen(OLY_DATA_SOURCE_DIR)+1));
            strcpy(path, getenv("HOME"));
            strcat(path, DIR_SEPARATOR);
            strcat(path, OLY_DATA_SOURCE_DIR);

            if (path != NULL)
            {
                errors = lt_dladdsearchdir(path);
            }
        }

        if (errors == 0)
        {
            errors = lt_dladdsearchdir(DATASOURCEDIR);
        }

        if (errors != 0)
        {
            last_error = lt_dlerror ();
        }

        ++initialised;

        return errors ? OLY_ERROR : OLY_OKAY;
    }

    last_error = multi_init_error;
    return OLY_ERROR;
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

    retval->ds_type             = ds_type;
    retval->direction           = OLY_DS_IN;
    retval->unused_settings     = 0x0;
    retval->required_settings   = 0x0;
    retval->locale              = NULL;
    retval->charset             = NULL;
    retval->buffer_size         = BUFSIZ;
    retval->ochar_buffer        = ocalloc(BUFSIZ, sizeof(OChar));
    retval->char_buffer         = ocalloc(BUFSIZ, sizeof(char));
    retval->converter           = NULL;
    if (COLLECTION_DATA_SOURCE == )
        retval->data            = 

/*    retval->init_function = NULL;
    retval->open_function = NULL;
    retval->delete_function = NULL; 
    */
    return retval;
}

OlyDataSourceFormat
get_data_type_format( DataSourceType ds_type )
{
    OlyDataSourceFormat format;

    switch (ds_type)
    {
        case SQLITE_FILE:
            format = TABLE_DATA_SOURCE;
            break;
        case YAML_FILE:
            format = COLLECTION_DATA_SOURCE;
            break;
        default:
            format = UNKNOWN_DATA_SOURCE_FORMAT;
            break;
    }
    return format;
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
    OlyStatus       status   = OLY_OKAY;
    UErrorCode      u_status = U_ZERO_ERROR;
    ds->charset = (char *)charset;
    if (ds->converter != NULL)
    {
        ucnv_close(ds->converter);
    }
    ds->converter = ucnv_open( charset, &u_status );
    
    if (U_FAILURE(u_status))
    {
    
        ucnv_close(ds->converter);
        status = OLY_ERR_LIB;
    }
    return status;
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

UConverter  *
get_ds_charset_converter(OlyDataSource *ds)
{
    return ds->converter;
}

