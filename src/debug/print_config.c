/* print_config.c - Debug function to print config as loaded. {{{
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
#include "oly/core.h"
#include "oly/config.h"
#include "pvt_config.h"
#include "oly/oly_dev.h"

static void zero_token_mark (TokenMark *zero_me);
void 
print_config( OlyStatus *status )
{
    yaml_parser_t        config_parser ;
    yaml_token_t         token;
    OlyConfig           *olyconf;
    TokenMark            token_marker;

    zero_token_mark(&token_marker);

    if ( *status != OLY_OKAY )
    {
        return;
    }
    else if ( config_file == NULL )
    {
        *status = OLY_ERR_CONFIG_FILE_NOT_FOUND ;
    }
    
    if ( yaml_parser_initialize(&config_parser) != 1 )
    {
        *status = OLY_ERR_LIBYAML_INIT;
         return;
    }
    /* TODO:
     * This encoding set may need to be dynamic at some point.  
     * since this is only in Linux right now on x86, we are fine,
     * but you never know! 
     */
    yaml_parser_set_encoding( &config_parser, YAML_UTF16LE_ENCODING );
    yaml_input_ofile( &config_parser , config_file );
    do {
        if (!yaml_parser_scan(&config_parser, &token)) 
        {
            printf("Parser error %d\n", config_parser.error);
            exit(EXIT_FAILURE);
        }
        switch(token.type)
        { 
            case YAML_STREAM_START_TOKEN:
                printf("STREAM START\n");
                break;
            case YAML_STREAM_END_TOKEN:
                printf("STREAM END\n");
                break;
            case YAML_VERSION_DIRECTIVE_TOKEN:
                printf("VERSION DIRECTIVE TOKEN\n");
                break;
            case YAML_TAG_DIRECTIVE_TOKEN:
                printf("TAG DIRECTIVE TOKEN\n");
                break;
            case YAML_DOCUMENT_START_TOKEN:
                printf("DOCUMENT START\n");
                break;
            case YAML_DOCUMENT_END_TOKEN:
                printf("DOCUMENT END\n");
                break;
            case YAML_BLOCK_SEQUENCE_START_TOKEN:
                printf("BLOCK SEQUENCE START\n");
                break;
            case YAML_BLOCK_MAPPING_START_TOKEN:
                printf("BLOCK MAPPING START\n");
                break;
            case YAML_BLOCK_END_TOKEN:
                puts("BLOCK END");
                break;
            case YAML_FLOW_SEQUENCE_START_TOKEN:
                puts("FLOW SEQUENCE START");
                break;
            case YAML_FLOW_SEQUENCE_END_TOKEN:
                puts("FLOW SEQUENCE END");
                break;
            case YAML_FLOW_MAPPING_START_TOKEN:
                puts("FLOW MAPPING START");
                break;
            case YAML_FLOW_MAPPING_END_TOKEN:
                puts("FLOW MAPPING END");
                break;
            case YAML_BLOCK_ENTRY_TOKEN:
                puts("[Block Entry]");
                break;
            case YAML_FLOW_ENTRY_TOKEN:
                puts("[Flow Entry]");
                break;
            case YAML_KEY_TOKEN:
                token_marker.is_key_token = 1;
                break;
            case YAML_VALUE_TOKEN:
                token_marker.is_value_token = 1;
                break;
            case YAML_ALIAS_TOKEN:
                puts("al - i - as TOKEN!");
                break;
            case YAML_ANCHOR_TOKEN:
                puts("ANCHOR TOKEN!");
                break;
            case YAML_TAG_TOKEN:
                puts("TAG TOKEN!");
                break;
            case YAML_SCALAR_TOKEN:
                if (token_marker.is_key_token == 1) 
                {
                    u_fprintf(u_stdout, "%s : ", token.data.scalar.value);
                }
                else
                {
                    u_fprintf(u_stdout, "%s\n", token.data.scalar.value);
                }
                zero_token_mark(&token_marker);
                break;
            default: 
                break;
        }
        if(token.type != YAML_STREAM_END_TOKEN)
        {
            yaml_token_delete(&token);
        }
    } while(token.type != YAML_STREAM_END_TOKEN) ;
    yaml_token_delete(&token);
  /* END new code */
    return;
}

/* neutralize the token marker */
static void
zero_token_mark (TokenMark *zero_me)
{
    zero_me->is_stream_start = 0;
    zero_me->is_stream_end = 0;
    zero_me->is_version_directive = 0;
    zero_me->is_tag_directive = 0;
    zero_me->is_document_start = 0;
    zero_me->is_document_end = 0;
    zero_me->is_block_sequence_start = 0;
    zero_me->is_block_mapping_start = 0;
    zero_me->is_block_end = 0;
    zero_me->is_flow_sequence_start = 0;
    zero_me->is_flow_sequence_end = 0;
    zero_me->is_flow_mapping_start = 0;
    zero_me->is_flow_mapping_end = 0;
    zero_me->is_block_entry_start = 0;
    zero_me->is_flow_entry_start = 0;
    zero_me->is_key_token = 0;
    zero_me->is_value_token = 0;
    zero_me->is_alias_token = 0;
    zero_me->is_anchor_token = 0;
    zero_me->is_tag_token = 0;
    zero_me->is_scalar_token = 0;
}

static int
ofile_read_handler(void *data, unsigned char *buffer, size_t size,
        size_t *size_read)
{
    yaml_parser_t *parser = data;

    *size_read = (u_file_read((UChar *)buffer, 
                    (int32_t)(size/sizeof(OChar)-1), 
                    (UFILE *)parser->input.file)*sizeof(OChar));
    if (parser->input.file != NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static void
yaml_input_ofile(yaml_parser_t *parser, OFILE *file)
{
    assert(parser); /* Non-NULL parser object expected. */
    assert(!parser->read_handler);  /* You can set the source only once. */
    assert(file);   /* Non-NULL file object expected. */

    parser->read_handler = ofile_read_handler;
    parser->read_handler_data = parser;

    parser->input.file = (FILE *)file;
}

/* find_config_file.  Looks for a file in any of the colon-separated
 * directories listed in the path argument. returns OlyStatus.  
 * May return one of these errors: OLY_ERR_BADARG (one of the 
 * arguments provided was invalid), OLY_ERR_FILE_NOT_FOUND (not successful 
 * finding the file).
 * If it recieves a status != OLY_OKAY, returns that error and does nothing 
 * else.
 */
#define DOT_CONFIG_NAME "/.olyrc"
#define ETC_CONFIG_NAME "/olyrc"

static OFILE *
find_config_file (OlyStatus *status) 
{
    char *path = strcat(strcpy(
            omalloc((strlen(getenv("HOME")) + strlen(DOT_CONFIG_NAME)+1)),
            getenv("HOME")), DOT_CONFIG_NAME),
            result[BUFSIZ], *token, *watch;
    const char  *basename = ETC_CONFIG_NAME,
                *sysconf_path = SYSCONFDIR;
    const char  *the_colon = ":";
    size_t       name_size = ( strlen(basename) + 1 ), 
                 result_len = BUFSIZ;
    OFILE       *return_file = NULL;
    void        *free_me;

    if (*status != OLY_OKAY)
    {
        return NULL;
    }
    
    if ( path != NULL )
    {
        return_file = u_fopen( path , "rb", NULL, NULL );
        free_me = (void *)path;
        OFREE(free_me);
    }

    if (return_file == NULL)
    {
        path = omalloc( (strlen(sysconf_path) + 1) );
        strcpy(path, sysconf_path);
        for ( token = strtok_r(path, the_colon, &watch); 
                ( token != NULL ); 
                token = strtok_r(NULL, the_colon, &watch) )
        {
            if ((token != NULL) && ((strlen(token) + name_size) < result_len))
            {
                strcpy( result, token );
                strcat( result, basename );
                return_file = u_fopen( result, "rb", 
                        char_default_locale(), char_default_charset() );
            }
            else
            {
                *status = OLY_ERR_FILE_NOT_FOUND;
                *result = '\0';
            }
        }
        free_me = (void *)path;
        OFREE(free_me);
    }
    return return_file;
}

