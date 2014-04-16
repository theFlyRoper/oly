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

static const ResourceData *init_main_config(void);
static OlyStatus load_config_file( OlyConfig **config, FILE *file );
static const ResourceData * config_data;
/* stores the type for each OlyConfigItem, in order */
static const OlyConfigValueType type[OLY_CONFIG_ITEM_MAX] =
{
    INT_VALUE,
    INT_VALUE,
    INT_VALUE,
    INT_VALUE
};
    
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
        fprintf(stderr, "ICU Error: %s.\n",
                u_errorName(u_status));
    }
#endif /* HAVE_UNICODE_URES_H */
    return status; 
}
{ 
    UErrorCode      u_status = U_ZERO_ERROR;
    int32_t         i = 0;
    UResourceBundle *subres = NULL;
    char            indent_buffer[BUFSIZ];
    
    for (i = 0; i<ures_getSize(res); i++) {
        subres = ures_getByIndex (res, i, subres, &u_status);
        if (U_FAILURE(u_status)) {
            fprintf(stderr, "Err: %s\n",
                    u_errorName(u_status));
            exit(1);
        }
        if (URES_NONE != flag_check(subres, flag)) {
            printf("%s%8i - %s: ", 
                    level_indent(indent_buffer, BUFSIZ, level), i, 
                    get_resource_type(subres));
            flag_res_display(subres, flag, level);
        }
    }
    printf("\n");
    ures_close(subres);
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

OlyStatus load_config( OlyConfig **config )
{
    OlyStatus    status = OLY_OKAY;
    size_t       length = strlen(SYSCONFDIR) + 1;
    char         path[BUFSIZ], filename[BUFSIZ], *token = NULL,
                *marker = NULL, sep[1] = ":", *config_file = "olyrc.yaml";
    bool         file_loaded = false;
    FILE        *file = NULL;
    
    if (length > BUFSIZ) 
    {
        status = OLY_ERR_BUFFER_OVERFLOW;
    }
    HANDLE_ERROR_AND_DIE(status);
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
        if (status == OLY_OKAY)
        {
            file_loaded = true;
        }
    }
    strncpy(path, SYSCONFDIR, (BUFSIZ-1));
    
    for (token = strtok_r(path, sep, &marker); token; 
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
    
    return status; 
}

OlyStatus load_config_file( OlyConfig **config, FILE *file )
{
    OlyStatus            status      = OLY_OKAY;
    OlyConfigItem        item = OLY_CONFIG_UNSET;
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
                printf("Config support for collections not implemented yet.\n");
                status = OLY_ERR_CONFIG_UNKNOWN;
                break;
            case YAML_ALIAS_EVENT:
                break;
            case YAML_SCALAR_EVENT:
                if ( item == OLY_CONFIG_UNSET )
                {
                    status = stage_node_key( data, 
                        (const char *)event.data.scalar.value,
                        (size_t)event.data.scalar.length );
                    have_key = 0x1;
                }
                else 
                {
                    status = enqueue_ds_node( data, event.data.scalar.value,
                            OLY_TAG_SCALAR_STRING);
                    INBOUND_NODE_CHECK(status, data);
                    have_key = 0x0;
                    item = OLY_CONFIG_UNSET;
                }
                break;
            case YAML_NO_EVENT: 
                break;
            default: 
                break;
        }
        yaml_event_delete(event);
    item = OLY_CONFIG_UNSET;
    } while ( event.type != YAML_STREAM_END_EVENT );
    return status; 
}

OlyStatus set_config_value( OlyConfig **config, OlyConfigItem item, 
    OlyConfigValue *value )
{

}

OlyStatus
find_config_item( const char *find_in, OlyConfigItem *item_out )
{
    UErrorCode u_status = U_ZERO_ERROR;
    const int32_t array_end = ures_countArrayItems(config_data, NULL, &u_status),
          str_length = BUFSIZ, i = 0;
    char  config_string[BUFSIZ], *config_dest = NULL;
    bool  found = false;
    if (U_FAILURE(u_status))
    {
        status = OLY_ERR_CONFIG_UNKNOWN;
        return status;
    }
    
    for (i = 0; ((i < array_end) && (found == false)); i++)
    {
        config_dest = ures_getUTF8StringByIndex(config_data, i, config_string,
                &str_length, FALSE, &u_status);
        if (config_dest == NULL){
            status = OLY_ERR_CONFIG_ITEM_NOT_FOUND;
            break;
        }
        if (U_FAILURE(u_status))
        {
            status = OLY_ERR_CONFIG_UNKNOWN;
            break;
        }
        if (strcmp((const char *)config_dest, find_in) == 0)
        {
            *item_out = (OlyConfigItem)i;
            found = true;
        }
        str_length = BUFSIZ;
    }
    if (found == false)
    {
        status = OLY_ERR_CONFIG_ITEM_NOT_FOUND;
    }
    return status;
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

