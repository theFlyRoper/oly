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
#include <yaml.h>
#include "sys/types.h"
#include "oly/core.h"

static OFILE *find_config_file (OlyStatus *status)  ;
void yaml_input_ofile(yaml_parser_t *parser, OFILE *file);
static int ofile_read_handler(void *data, 
        unsigned char *buffer, size_t size, size_t *size_read);

#define NO_STATE 0
#define IS_KEY 1
#define IS_VALUE 2

/* using defines and a bit flipper because it is easier */
OlyConfig *
load_config( OlyStatus *status )
{
    FILE *config_file = find_config_file(status);
    yaml_parser_t        config_parser ;
    yaml_event_t         event;
    OlyConfig           *olyconf;
    if ( *status != OLY_OKAY )
    {
        return NULL;
    }
    else if ( config_file == NULL )
    {
        *status = OLY_ERR_CONFIG_FILE_NOT_FOUND ;
    }
    if ( yaml_parser_initialize(&config_parser) != 1 )
    {
        *status = OLY_ERR_LIBYAML_INIT;
         return NULL;
    }
    
    yaml_input_ofile( &config_parser , config_file );
    
    while(event.type != YAML_STREAM_END_EVENT)
    {
        if (!yaml_parser_parse(&config_parser, &event)) 
        {
            printf("Parser error %d\n", config_parser.error);
            exit(EXIT_FAILURE);
        }
        switch(event.type)
        { 
            case YAML_NO_EVENT: 
                break;
            /* Stream start/end */
            case YAML_STREAM_START_EVENT: 
                break;
            case YAML_STREAM_END_EVENT:   
                break;
            /* Block delimeters */
            case YAML_DOCUMENT_START_EVENT: 
                break;
            case YAML_DOCUMENT_END_EVENT:   
                break;
            case YAML_SEQUENCE_START_EVENT: 
                break;
            case YAML_SEQUENCE_END_EVENT:   
                break;
            case YAML_MAPPING_START_EVENT:  
                break;
            case YAML_MAPPING_END_EVENT:    
                break;
            /* Data */
            case YAML_ALIAS_EVENT:  
                break;
            case YAML_SCALAR_EVENT: 
                u_fprintf(u_stdout, "Got scalar\nanchor = %s\ntag = %s\nvalue = %s\n ", 
                        event.data.scalar.anchor,
                        event.data.scalar.tag,
                        event.data.scalar.value
                        );
                break;
        }
        if(event.type != YAML_STREAM_END_EVENT)
        yaml_event_delete(&event);
    } 
    yaml_event_delete(&event);
  /* END new code */
    return olyconf;
}

#define DOT_CONFIG_NAME "/.olyrc"
#define ETC_CONFIG_NAME "/olyrc"
static int
ofile_read_handler(void *data, unsigned char *buffer, size_t size,
        size_t *size_read)
{
    yaml_parser_t *parser = data;

    *size_read = (u_fread((UChar *)buffer, 
                    (int32_t)(size/sizeof(OChar)-1), 
                    parser->input.file)*sizeof(OChar));
    return !ferror(parser->input.file);
}

static void
yaml_input_ofile(yaml_parser_t *parser, OFILE *file)
{
    assert(parser); /* Non-NULL parser object expected. */
    assert(!parser->read_handler);  /* You can set the source only once. */
    assert(file);   /* Non-NULL file object expected. */

    parser->read_handler = ofile_read_handler;
    parser->read_handler_data = parser;

    parser->input.file = file;
}

/* find_config_file.  Looks for a file in any of the colon-separated
 * directories listed in the path argument. returns OlyStatus.  
 * May return one of these errors: OLY_ERR_BADARG (one of the 
 * arguments provided was invalid), OLY_ERR_FILE_NOT_FOUND (not successful 
 * finding the file).
 * If it recieves a status != OLY_OKAY, returns that error and does nothing 
 * else.
 *
 * If status is OLY_OKAY at the end, the file name in result can be opened
 * to read, if nothing else.
 */

static OFILE *
find_config_file (OlyStatus *status) 
{
    char *path = strcat(strcpy(
            omalloc((strlen(getenv("HOME")) + strlen(DOT_CONFIG_NAME)+1)),
            getenv("HOME")), DOT_CONFIG_NAME),
            result[BUFSIZ], 
            *token, *watch;;
    const char  *the_colon = ":";
    size_t       path_size, name_size = ( strlen(basename) + 1 ), 
                 result_len = BUFSIZ;
    OFILE        *return_file = NULL;
    void        *free_me;


    if (*status != OLY_OKAY)
    {
        return *status;
    }
    
    if ( path != NULL )
    {
        return_file = u_fopen( path , "rb", NULL, NULL );
        free_me = (void *)path;
        OFREE(free_me);
    }

    if (return_file == NULL)
    {
        for ( token = strtok_r(path, the_colon, &watch); 
                ( token != NULL ); 
                token = strtok_r(NULL, the_colon, &watch) )
        {
            if ((token != NULL) && ((strlen(token) + name_size) < result_len))
            {
                strcpy( result, token );
                strcat( result, basename );
                return_file = u_fopen(result, "rb", get_default_locale(), 
                    get_default_charset());
            }
            else
            {
                *status = OLY_ERR_FILE_NOT_FOUND;
                *result = '\0';
            }
        }
    }
    return return_file;
}

