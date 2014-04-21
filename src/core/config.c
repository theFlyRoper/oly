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
#include "yaml.h"
#include "oly/core.h"
#include "oly/olytypes.h"
#include <stdbool.h>
#include "pvt_config.h"

/* ICU config data stored in the language data package */
static const ResourceData * config_data;

static const ResourceData *init_main_config(void);
static OlyStatus count_config_file( OlyConfig **config, FILE *file )
static OlyStatus load_config_file( OlyConfig **config, FILE *file );
static OlyStatus get_int_config(OlyConfig *config, OlyConfigItem item, int64_t *output );
static OlyStatus find_config_item(unsigned char *key, OlyConfigItem *item);

OlyStatus init_config(void)
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

static const ResourceData * init_main_config(void)
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

OlyStatus load_config( OlyConfig **config )
{
    OlyStatus    status = init_config();
    size_t       length = strlen(SYSCONFDIR) + 1;
    char         path[BUFSIZ], filename[BUFSIZ], *token = NULL,
                *marker = NULL, *sep = (char *)":", *config_file = (char *)"olyrc";
    bool         file_loaded = false;
    FILE        *file = NULL;
    if (length > BUFSIZ) 
    {
        status = OLY_ERR_BUFFER_OVERFLOW;
    }
    if ( file_loaded == true )
    {
        status = OLY_ERR_INIT;
        return status;
    }
    path[BUFSIZ-1] = '\0';

    strcpy(filename, getenv("HOME"));
    strcat(filename, "/.");
    strcat(filename, config_file);
    file = fopen(filename, "rb");
    if ( file == NULL ) 
    {
        status = OLY_ERR_CONFIG_FILE_NOT_FOUND;
    }

    if (status == OLY_OKAY)
    {
        status = load_config_file( config, file );
        if (status != OLY_OKAY)
        {
            status = OLY_ERR_CONFIG_FILE_NOT_FOUND;
        }
        else
        {
            file_loaded = true;
        }

    }
    strncpy(path, SYSCONFDIR, (BUFSIZ-1));
    token = strtok_r(path, sep, &marker);
    for (; (token != NULL); 
            token = strtok_r(NULL, sep, &marker))
    {
        if (status == OLY_ERR_CONFIG_FILE_NOT_FOUND) 
        {
            status = OLY_OKAY;
        }

        strcpy(filename, token);
        strcat(filename, "/");
        strcat(filename, config_file);
        file = fopen(filename, "rb");
        if( file == NULL ) 
        {
            status = OLY_ERR_CONFIG_FILE_NOT_FOUND;
        }

        if (status == OLY_OKAY)
        {
            status = load_config_file( config, file );
            if (status == OLY_OKAY)
            {
                file_loaded = true;
            }
        }
    }
    if ( file_loaded != true )
    {
        status = OLY_ERR_CONFIG_FILE_NOT_FOUND;
    }
    else
    {
        status = OLY_OKAY;
    }
    
    return status; 
}

OlyStatus load_config_file( OlyConfig **config, FILE *file )
{
    OlyStatus            status = OLY_OKAY;
    OlyConfigItem        item   = OLY_CONFIG_UNSET;

    yaml_parser_t parser ;
    yaml_event_t  event;
    if ( yaml_parser_initialize( &parser ) != 1 )
    {
        status = OLY_ERR_LIBYAML_INIT;
        return status;
    };
    yaml_parser_set_input_file( &parser, file);
    
    do {
        if (!yaml_parser_parse(&parser, &event))
        {
            printf("Parser error %d\n", parser.error);
            exit(EXIT_FAILURE);
        }
        switch(event.type)
        {
            /* Stream start/end */
            case YAML_STREAM_START_EVENT: 
                break;
            case YAML_STREAM_END_EVENT:   
                break;
            case YAML_DOCUMENT_START_EVENT: 
                break;
            case YAML_DOCUMENT_END_EVENT:   
                break;
            case YAML_SEQUENCE_START_EVENT:
            case YAML_SEQUENCE_END_EVENT:
            case YAML_MAPPING_START_EVENT:  
            case YAML_MAPPING_END_EVENT:    
                status = OLY_OKAY;
                break;
            case YAML_ALIAS_EVENT:
                break;
            case YAML_SCALAR_EVENT:
                if ( item == OLY_CONFIG_UNSET )
                {
                    status = find_config_item( event.data.scalar.value, &item );
                    if (status == OLY_ERR_CONFIG_UNKNOWN)
                    {
                        status = OLY_OKAY;
                    }
                    HANDLE_STATUS_AND_RETURN(status);
                }
                else 
                {
                    printf("FOUND ITEM %i, value %s\n", item, event.data.scalar.value);
                    item = OLY_CONFIG_UNSET;
                }
                break;
            case YAML_NO_EVENT: 
                break;
            default: 
                break;
        }
        if ( event.type != YAML_STREAM_END_EVENT )
        {
            yaml_event_delete(&event);
        }
    } while ( event.type != YAML_STREAM_END_EVENT );
    yaml_event_delete(&event);
    return status; 
}

OlyStatus
get_main_config_int(OlyConfigItem item, int64_t *output )
{
    static OlyConfig *config = NULL;
    OlyStatus status = OLY_OKAY;

    if (config == NULL)
    {
        status = get_main_config( &config );
    }

    status = get_int_config(config, item, output);
    return status;
}

OlyStatus
get_int_config(OlyConfig *config, OlyConfigItem item, int64_t *output )
{
    OlyStatus status = OLY_OKAY;

    switch (item)
    {
        case OLY_CONFIG_BOUNDARY_BUFFER_MAX:
            *output = BUFSIZ;
            break;
        default:
            *output = BUFSIZ;
            break;
    }
    return status;
}

OlyStatus find_config_item( unsigned char *key, OlyConfigItem *item)
{
    OlyStatus   status = OLY_ERR_CONFIG_UNKNOWN;
    OChar       config[BUFSIZ], *config_item = NULL;
    int32_t     len = 0;
    UErrorCode  u_status = U_ZERO_ERROR;
    
    u_strFromUTF8(config, BUFSIZ, NULL, (const char *)key, -1, &u_status);
    if (U_FAILURE(u_status)) 
    {
        printf("ICU cannot find config item.  Status: %s.\n",
                u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    for(*item = OLY_CONFIG_UNSET; *item < OLY_CONFIG_ITEM_MAX; (*item)++)
    {
        config_item = (OChar *)ures_getStringByIndex( config_data, 
            *item, &len, &u_status );
        if (U_FAILURE(u_status)) 
        {
            printf("ICU cannot find config item.  Status: %s.\n",
                    u_errorName(u_status));
            exit(EXIT_FAILURE);
        }
        if (u_strcmp( config, config_item ) == 0)
        {
            status = OLY_OKAY;
            break;
        }
    }
    if (*item == OLY_CONFIG_ITEM_MAX)
    {
        *item = OLY_CONFIG_UNSET;
    }
    return status;
}

