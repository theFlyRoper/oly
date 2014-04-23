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
#include <limits.h>
#include <stdbool.h>
#include <yaml.h>

#include "oly/core.h"
#include "oly/olytypes.h"
#include "oly/tag.h"
#include "core/pvt_config.h"

static const OlyConfig oly_main_default[] = 
{
    { {{NULL}, OLY_TAG_TYPE_UNSET}, BUFSIZ,  OLY_TAG_SCALAR_UINT },
    { {{NULL}, OLY_TAG_TYPE_UNSET}, (BUFSIZ*2),  OLY_TAG_SCALAR_UINT },
    { {{NULL}, OLY_TAG_TYPE_UNSET}, (BUFSIZ/2),  OLY_TAG_SCALAR_UINT }
};
/* ICU config data stored in the language data package.  A key list. */
static const ResourceData *config_data;

static OlyStatus init_config(void);
static const ResourceData *init_main_config(void);
static OlyStatus copy_config_item(OlyConfig *dest, const OlyConfig *source);
static OlyStatus load_config_file( OlyConfig *config, FILE *file );
static OlyStatus load_main_defaults( OlyConfig *config );
static OlyStatus get_int_config(OlyConfig *config, int item, long int *output );
static OlyStatus find_config_item(unsigned char *key, OlyConfigMainItem *item);

/* sets up a single already allocated config item. */
OlyStatus open_config_item( OlyConfig *config )
{
    OlyStatus status = OLY_OKAY;
    config->value.value.string_value = NULL;
    config->min = 0;
    config->type = OLY_TAG_TYPE_UNSET;
    return status;
}

void close_config_item( OlyConfig **config )
{
    if (((*config)->type == OLY_TAG_SCALAR_STRING) && ((*config)->value.value.string_value !=  NULL))
    {
        free((*config)->value.value.string_value);
    }
    (*config)->value.value.string_value = NULL;
    (*config)->min = 0;
    return;
}

/* for each record in the config passed to it, sets if that record is unset. */
OlyStatus load_config_file( OlyConfig *config, FILE *file )
{
    OlyStatus            status = OLY_OKAY;
    OlyConfigMainItem    item   = OLY_CONFIG_MAIN_UNSET;

    yaml_parser_t parser;
    yaml_event_t  event ;
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
                if ( item == OLY_CONFIG_MAIN_UNSET )
                {
                    status = find_config_item( event.data.scalar.value, &item );
                    if ( status == OLY_ERR_CONFIG_UNKNOWN )
                    {
                        status = OLY_OKAY;
                    }
                    HANDLE_STATUS_AND_RETURN(status);
                }
                else 
                {
                    if ( (config[item]).type == OLY_TAG_TYPE_UNSET )
                    {
                        /* 
                        status = get_config_tag_type(oly_main_default[item], &(config[item]).type);
                        HANDLE_STATUS_AND_RETURN(status);
                        status = set_config_item( event.data.scalar.value, &config[item], item);
                        HANDLE_STATUS_AND_RETURN(status); */
                        status = OLY_OKAY;
                    }
                    item = OLY_CONFIG_MAIN_UNSET;
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

OlyStatus get_config_tag_type( OlyConfigMainItem item, OlyTagType *type )
{
    OlyStatus status = OLY_OKAY;
    *type = oly_main_default[(int)item].type;
    return status; 
}

OlyStatus set_config_item( char *value, OlyConfig *config_item, OlyConfigMainItem item)
{
    OlyStatus status = OLY_OKAY;
    int i = (int)item;
    
    config_item[i].value = oly_main_default[i].value;

    return status;
}

static OlyStatus 
load_main_defaults( OlyConfig *config )
{
    OlyStatus status = OLY_OKAY;
    int i = 0;
    for ( i = 0; i < OLY_CONFIG_MAIN_UNSET; i++ )
    {
        if ( (config[i]).type == OLY_TAG_TYPE_UNSET )
        {
            status = copy_config_item(&config[i], &oly_main_default[i]);
        }
        HANDLE_STATUS_AND_RETURN(status);
    }
    return status;
}

OlyStatus copy_config_item(OlyConfig *dest, const OlyConfig *source)
{
    OlyStatus status = OLY_OKAY;
    return status;
}

OlyStatus
get_main_config_int(OlyConfigMainItem item, long int *output )
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

OlyStatus load_main_config( OlyConfig **config )
{
    OlyStatus        status = init_config();
    size_t           length = strlen(SYSCONFDIR) + 1;
    char             path[BUFSIZ], filename[BUFSIZ], *token = NULL,
                    *marker = NULL, *sep = (char *)":", *config_file = (char *)"olyrc";
    bool             file_loaded = false;
    FILE            *file = NULL;
    int              i = 0;
    OlyConfig   *oly_conf = (OlyConfig *)omalloc(sizeof(OlyConfig)*OLY_CONFIG_MAIN_UNSET);

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
    for ( i = 0; i < OLY_CONFIG_MAIN_UNSET; i++ )
    {
        open_config_item(&(oly_conf[i]));
    }
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
        status = load_config_file( oly_conf, file );
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
            status = load_config_file( oly_conf, file );
            if (status == OLY_OKAY)
            {
                file_loaded = true;
            }
        }
    }
    status = load_main_defaults(oly_conf);
    if (( file_loaded != true ) && ( status == OLY_OKAY ))
    {
        status = OLY_ERR_CONFIG_FILE_NOT_FOUND;
    }
    else
    {
        status = OLY_OKAY;
    }
    return status; 
}

OlyStatus
get_int_config(OlyConfig *config, int item, long int *output )
{
    OlyStatus status = OLY_OKAY;

    switch (item)
    {
        case OLY_CONFIG_MAIN_BOUNDARY_BUFFER_MAX:
            *output = BUFSIZ;
            break;
        default:
            *output = BUFSIZ;
            break;
    }
    return status;
}

OlyStatus find_config_item( unsigned char *key, OlyConfigMainItem *item)
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
    for ( *item = 0; *item < OLY_CONFIG_MAIN_UNSET; (*item)++ )
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
    return status;
}

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

void free_main_config(void)
{
    return;
}

