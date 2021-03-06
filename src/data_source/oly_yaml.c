/* oly_yaml.c - Oly bindings for YAML.  License GPL 2+{{{
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
#include "pvt_data_source.h"
#include "oly/doc_data_source.h"
#include "oly/oly_dev.h"
#include "oly/core.h"

static void yaml_input_ofile(yaml_parser_t *parser, OFILE *file);
static int ofile_read_handler(void *data, 
        unsigned char *buffer, size_t size, size_t *size_read);
static void zero_token_mark (YAMLTokenMark *zero_me);
static YAMLTokenMark *new_token_mark (void);
static void print_preflight_count(YAMLTokenMark *token_mark, int level);
static YAMLTokenMark *preflight_yaml( OlyStatus *status , OlyDataSource *ds );
static void print_yaml( OlyStatus *status , OlyDataSource *ds )

void
load_yaml( OlyStatus *status , OlyDataSource *ds )
{
    OFILE               *yaml_file  = u_fopen( ds->filename, "rd", ds->locale, ds->encoding );
    yaml_parser_t        config_parser ;

    yaml_event_t         event;
/*    yaml_token_t         token; */
    YAMLTokenMark       *token_mark = new_token_mark();
    /* char                 ultrabuffer[BUFSIZ]; */

    zero_token_mark( token_mark );

    if ( *status != OLY_OKAY )
    {
        return ;
    }
    else if ( yaml_file == NULL )
    {
        *status = OLY_ERR_FILE_NOT_FOUND ;
    }
    
    if ( yaml_parser_initialize(&config_parser) != 1 )
    {
        *status = OLY_ERR_LIBYAML_INIT;
         return ;
    }
    /* TODO:
     * This encoding set may need to be dynamic at some point.  
     * since this is only in Linux right now on x86, we are fine,
     * but you never know! 
     */
    yaml_parser_set_encoding( &config_parser, YAML_UTF16LE_ENCODING );
    yaml_input_ofile( &config_parser , yaml_file );
    do {
        if (!yaml_parser_parse(&config_parser, &event)) 
        {
            printf( "Parser error %d, %s\n", config_parser.error, config_parser.problem );
            exit(EXIT_FAILURE);
        }

        switch(event.type)
        { 
        case YAML_NO_EVENT: puts("No event!"); break;
        /* Stream start/end */
        case YAML_STREAM_START_EVENT: 
            puts("<p>STREAM START"); 
            break;
        case YAML_STREAM_END_EVENT:   
            puts("STREAM END</p>");   
            break;
        /* Block delimeters */
        case YAML_DOCUMENT_START_EVENT: puts("<b>Start Document</b></br>"); break;
        case YAML_DOCUMENT_END_EVENT:   puts("<b>End Document</b></br>");   break;
        case YAML_SEQUENCE_START_EVENT: puts("<b>Start Sequence</b></br>"); break;
        case YAML_SEQUENCE_END_EVENT:   puts("<b>End Sequence</b></br>");   break;
        case YAML_MAPPING_START_EVENT:  puts("<b>Start Mapping</b></br>");  break;
        case YAML_MAPPING_END_EVENT:    puts("<b>End Mapping</b></br>");    break;
        /* Data */
        case YAML_ALIAS_EVENT:  printf("Got alias (anchor %s)</br>", event.data.alias.anchor); break;
        case YAML_SCALAR_EVENT: printf("Got scalar (value %s)</br>", event.data.scalar.value); break;
        }
        if(event.type != YAML_STREAM_END_EVENT)
        yaml_event_delete(&event);
    } while(event.type != YAML_STREAM_END_EVENT);
    yaml_event_delete(&event);
    /*
        print_stdout_char_color( WHITE, MAGENTA, DIM, "[VERSION DIRECTIVE]\n" );
        print_stdout_char_color( WHITE, CYAN, DIM, "[TAG DIRECTIVE]\n" );
        print_stdout_char_color( YELLOW, BLUE, DIM, "[Block Sequence Start]\n" );
    */
    return ;
}

/* preflight_yaml counts the number of nodes and OChar items needed for each
 * level of the document representation in question. */
YAMLTokenMark *
preflight_yaml( OlyStatus *status , OlyDataSource *ds )
{
    OFILE *yaml_file = u_fopen( ds->filename, "rd", ds->locale, ds->encoding );
    yaml_parser_t        config_parser ;
    yaml_token_t         token;
    YAMLTokenMark           *token_mark = new_token_mark();

    if ( *status != OLY_OKAY )
    {
        return NULL;
    }
    else if ( yaml_file == NULL )
    {
        *status = OLY_ERR_FILE_NOT_FOUND ;
    }
    
    if ( yaml_parser_initialize(&config_parser) != 1 )
    {
        *status = OLY_ERR_LIBYAML_INIT;
         return NULL;
    }
    yaml_parser_set_encoding( &config_parser, YAML_UTF16LE_ENCODING );
    yaml_input_ofile( &config_parser , yaml_file );
    if ( count_this_level( token_mark, &config_parser, &token ) != OLY_OKAY )
    {
        *status = OLY_ERR_YAML_PARSE;
    };
    yaml_token_delete(&token);
    return token_mark;
}

OlyStatus 
count_this_level( YAMLTokenMark *token_mark, 
    yaml_parser_t *config_parser, yaml_token_t *token)
{
    OlyStatus           status = OLY_OKAY;
    do {
        if (!yaml_parser_scan(config_parser, token)) 
        {
            printf("Parser error %d\n", config_parser->error);
            exit(EXIT_FAILURE);
        }
        switch(token->type)
        { 
            case YAML_STREAM_START_TOKEN:
            case YAML_VERSION_DIRECTIVE_TOKEN:
            case YAML_TAG_DIRECTIVE_TOKEN:
            case YAML_DOCUMENT_START_TOKEN:
            case YAML_DOCUMENT_END_TOKEN:
                break;
            case YAML_FLOW_MAPPING_START_TOKEN: 
            case YAML_BLOCK_MAPPING_START_TOKEN: 
            case YAML_BLOCK_ENTRY_TOKEN:
            case YAML_FLOW_ENTRY_TOKEN:
                if (token_mark->is_value_token == 1) 
                {
                    token_mark->subnode = new_token_mark();
                    count_this_level(token_mark->subnode, config_parser, token);
                }
                break;
            case YAML_FLOW_SEQUENCE_START_TOKEN: 
            case YAML_BLOCK_SEQUENCE_START_TOKEN:            
                break;
            case YAML_FLOW_SEQUENCE_END_TOKEN:
            case YAML_FLOW_MAPPING_END_TOKEN:
            case YAML_BLOCK_END_TOKEN:
            case YAML_STREAM_END_TOKEN:
                zero_token_mark(token_mark);
                return status;
                break;
            case YAML_KEY_TOKEN:
                (token_mark->count_nodes)++;
                token_mark->is_key_token = 1;
                break;
            case YAML_VALUE_TOKEN:
                token_mark->is_value_token = 1;
                break;
            case YAML_ALIAS_TOKEN:
            case YAML_ANCHOR_TOKEN:
            case YAML_TAG_TOKEN:
                break;
            case YAML_SCALAR_TOKEN:
                token_mark->count_ochars += ( strlen(
                            (const char *)token->data.scalar.value)+1 );
                zero_token_mark(token_mark);
                break;
            default: 
                break;
        }
        if(token->type != YAML_STREAM_END_TOKEN)
        {
            yaml_token_delete(token);
        }
    } while(token->type != YAML_STREAM_END_TOKEN) ;
    return status ;
}

/* prints the counts recieved from the counts_this_level function stored in the token_mark */

void
print_preflight_count(YAMLTokenMark *token_mark, int level)
{
    int i = 0;
    for (i = 0; (i<level); i++)
    {
        printf("  ");
    }
    printf("Node count/OChar count: %i/%i\n",
            token_mark->count_nodes,
            token_mark->count_ochars
            );
    if (token_mark->subnode != NULL) 
    {
        print_preflight_count(token_mark->subnode, (level + 1));
    }
}

void
print_yaml( OlyStatus *status , OlyDataSource *ds )
{
    OFILE *yaml_file   = u_fopen( ds->filename, "rd", ds->locale, ds->encoding );
    yaml_parser_t        config_parser ;
    yaml_token_t         token;
    YAMLTokenMark           *token_mark = new_token_mark();
    char                 ultrabuffer[BUFSIZ];

    zero_token_mark(token_mark);

    if ( *status != OLY_OKAY )
    {
        return ;
    }
    else if ( yaml_file == NULL )
    {
        *status = OLY_ERR_FILE_NOT_FOUND ;
    }
    
    if ( yaml_parser_initialize(&config_parser) != 1 )
    {
        *status = OLY_ERR_LIBYAML_INIT;
         return ;
    }
    /* TODO:
     * This encoding set may need to be dynamic at some point.  
     * since this is only in Linux right now on x86, we are fine,
     * but you never know! 
     */
    yaml_parser_set_encoding( &config_parser, YAML_UTF16LE_ENCODING );
    yaml_input_ofile( &config_parser , yaml_file );
    do {
        if (!yaml_parser_scan(&config_parser, &token)) 
        {
            printf("Parser error %d\n", config_parser.error);
            exit(EXIT_FAILURE);
        }
        switch(token.type)
        { 
            case YAML_STREAM_START_TOKEN:
                print_stdout_char_color( BLACK, WHITE, DIM, "[STREAM START]\n" );
                break;
            case YAML_STREAM_END_TOKEN:
                print_stdout_char_color( BLACK, WHITE, DIM, "[STREAM END]\n" );
                break;
            case YAML_VERSION_DIRECTIVE_TOKEN:
                print_stdout_char_color( WHITE, MAGENTA, DIM, "[VERSION DIRECTIVE]\n" );
                break;
            case YAML_TAG_DIRECTIVE_TOKEN:
                print_stdout_char_color( WHITE, CYAN, DIM, "[TAG DIRECTIVE]\n" );
                break;
            case YAML_DOCUMENT_START_TOKEN:
                print_stdout_char_color( MAGENTA, BLACK, DIM, "[Document Start]\n" );
                break;
            case YAML_DOCUMENT_END_TOKEN:
                print_stdout_char_color( MAGENTA, BLACK, DIM, "[Document End]\n" );
                break;
            case YAML_BLOCK_SEQUENCE_START_TOKEN:
                print_stdout_char_color( YELLOW, BLUE, DIM, "[Block Sequence Start]\n" );
                break;
            case YAML_BLOCK_MAPPING_START_TOKEN:
                if (token_mark->is_value_token == 1) 
                {
                    print_stdout_char_color(BLUE, BLACK, BRIGHT, 
                            "\n[Block Mapping START]\n");
                }
                else
                {
                    print_stdout_char_color(BLUE, BLACK, BRIGHT, 
                        "[Block Mapping START]\n");
                }
                break;
            case YAML_BLOCK_END_TOKEN:
                print_stdout_char_color(RED, BLACK, BRIGHT, "[Block End]\n" );
                break;
            case YAML_FLOW_SEQUENCE_START_TOKEN:
                print_stdout_char_color( YELLOW, BLUE, DIM, "[Flow Sequence Start]\n" );
                break;
            case YAML_FLOW_SEQUENCE_END_TOKEN:
                print_stdout_char_color( YELLOW, BLUE, DIM, "[Flow Sequence End]\n" );
                break;
            case YAML_FLOW_MAPPING_START_TOKEN:
                print_stdout_char_color( BLUE, RED, BRIGHT, "[Flow Mapping Start]\n" );
                break;
            case YAML_FLOW_MAPPING_END_TOKEN:
                print_stdout_char_color( BLUE, RED, BRIGHT, "[Flow Mapping End]\n" );
                break;
            case YAML_BLOCK_ENTRY_TOKEN:
                print_stdout_char_color( BLUE, BLACK, BRIGHT, "\n[Block Entry]" );
                break;
            case YAML_FLOW_ENTRY_TOKEN:
                print_stdout_char_color( YELLOW, BLACK, BRIGHT, "\n[Flow Entry]" );
                break;
            case YAML_KEY_TOKEN:
                token_mark->is_key_token = 1;
                break;
            case YAML_VALUE_TOKEN:
                token_mark->is_value_token = 1;
                break;
            case YAML_ALIAS_TOKEN:
                sprintf(ultrabuffer, "%s\n", token.data.alias.value);
                print_stdout_char_color( YELLOW, BLACK, BRIGHT, ultrabuffer );
                break;
            case YAML_ANCHOR_TOKEN:
                sprintf(ultrabuffer, "\n[Anchor Token] %s\n", token.data.anchor.value);
                print_stdout_char_color( GREEN, BLACK, BRIGHT, ultrabuffer);
                break;
            case YAML_TAG_TOKEN:
                sprintf(ultrabuffer, "\n[Tag Token] handle: %s, suffix: %s\n", token.data.tag.handle, token.data.tag.suffix);
                print_stdout_char_color( CYAN, BLACK, BRIGHT, ultrabuffer);
                break;
            case YAML_SCALAR_TOKEN:
                if (token_mark->is_key_token == 1) 
                {
                    u_fprintf(u_stdout, "%s : ", token.data.scalar.value);
                }
                else
                {
                    u_fprintf(u_stdout, "%s\n", token.data.scalar.value);
                }
                zero_token_mark(token_mark);
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
    return ;
}

/* allocate a new token mark structure. */
static YAMLTokenMark *
new_token_mark (void)
{
    YAMLTokenMark *retval = (YAMLTokenMark *)omalloc(sizeof(YAMLTokenMark));
    retval->count_nodes = 0;
    retval->count_ochars = 0;
    retval->subnode = NULL ;
    zero_token_mark(retval);
    return retval;
}

/* neutralize the token marker */
static void
zero_token_mark (YAMLTokenMark *zero_me)
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
ofile_read_handler(void *data, unsigned char *buffer, size_t size, size_t *size_read)
{
    yaml_parser_t *parser = data;
    
    *size_read = (u_file_read((UChar *)buffer, (int32_t)(size/sizeof(OChar)-1), 
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

