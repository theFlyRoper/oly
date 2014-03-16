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
#include "oly/doc_data_source.h"
#include "oly/oly_dev.h"
#include "oly/core.h"

static OFILE *find_config_file (OlyStatus *status)  ;
static void yaml_input_ofile(yaml_parser_t *parser, OFILE *file);
static int ofile_read_handler(void *data, 
        unsigned char *buffer, size_t size, size_t *size_read);
static void zero_token_mark (TokenMark *zero_me);
static TokenMark *new_token_mark (void);
OlyStatus count_this_level( TokenMark *token_mark, 
    yaml_parser_t *config_parser, yaml_token_t *token);
static void print_preflight_count(TokenMark *token_mark, int level);

#define NO_STATE 0
#define IS_KEY 1
#define IS_VALUE 2

OlyConfig *
load_config( OlyStatus *status )
{
    OFILE *config_file = find_config_file(status);
    yaml_parser_t        config_parser ;
    yaml_token_t         token;
    OlyConfig           *olyconf;
    TokenMark           *token_mark = new_token_mark();

    zero_token_mark(token_mark);

    if ( *status != OLY_OKAY )
    {
        return NULL;
    }
    else if ( config_file == NULL )
    {
        *status = OLY_ERR_FILE_NOT_FOUND ;
    }
    
    if ( yaml_parser_initialize(&config_parser) != 1 )
    {
        *status = OLY_ERR_LIBYAML_INIT;
         return NULL;
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
                break;
            case YAML_STREAM_END_TOKEN:
                break;
            case YAML_VERSION_DIRECTIVE_TOKEN:
                break;
            case YAML_TAG_DIRECTIVE_TOKEN:
                break;
            case YAML_DOCUMENT_START_TOKEN:
                break;
            case YAML_DOCUMENT_END_TOKEN:
                break;
            case YAML_BLOCK_SEQUENCE_START_TOKEN:
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
                print_stdout_char_color(RED, BLACK, BRIGHT, 
                        "[Block END]\n");
                break;
            case YAML_FLOW_SEQUENCE_START_TOKEN:
                break;
            case YAML_FLOW_SEQUENCE_END_TOKEN:
                break;
            case YAML_FLOW_MAPPING_START_TOKEN:
                break;
            case YAML_FLOW_MAPPING_END_TOKEN:
                break;
            case YAML_BLOCK_ENTRY_TOKEN:
                print_stdout_char_color(BLUE, BLACK, BRIGHT, "\n[Block Entry]");
                break;
            case YAML_FLOW_ENTRY_TOKEN:
                break;
            case YAML_KEY_TOKEN:
                token_mark->is_key_token = 1;
                break;
            case YAML_VALUE_TOKEN:
                token_mark->is_value_token = 1;
                break;
            case YAML_ALIAS_TOKEN:
                break;
            case YAML_ANCHOR_TOKEN:
                break;
            case YAML_TAG_TOKEN:
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
    return olyconf;
}

/* preflight_config counts the number of nodes and OChar items needed for each
 * level of the document representation in question. */
TokenMark *
preflight_config( OlyStatus *status )
{
    OFILE *config_file = find_config_file(status);
    yaml_parser_t        config_parser ;
    yaml_token_t         token;
    TokenMark           *token_mark = new_token_mark();

    if ( *status != OLY_OKAY )
    {
        return NULL;
    }
    else if ( config_file == NULL )
    {
        *status = OLY_ERR_FILE_NOT_FOUND ;
    }
    
    if ( yaml_parser_initialize(&config_parser) != 1 )
    {
        *status = OLY_ERR_LIBYAML_INIT;
         return NULL;
    }
    yaml_parser_set_encoding( &config_parser, YAML_UTF16LE_ENCODING );
    yaml_input_ofile( &config_parser , config_file );
    if ( count_this_level( token_mark, &config_parser, &token ) != OLY_OKAY )
    {
        *status = OLY_ERR_YAML_PARSE;
    };
    yaml_token_delete(&token);
    return token_mark;
}

OlyStatus 
count_this_level( TokenMark *token_mark, 
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
            case YAML_FLOW_SEQUENCE_START_TOKEN: 
            case YAML_BLOCK_MAPPING_START_TOKEN: 
            case YAML_BLOCK_SEQUENCE_START_TOKEN:            
            case YAML_BLOCK_ENTRY_TOKEN:
            case YAML_FLOW_ENTRY_TOKEN:
                if (token_mark->is_value_token == 1) 
                {
                    token_mark->subnode = new_token_mark();
                    count_this_level(token_mark->subnode, config_parser, token);
                }
                break;
            case YAML_BLOCK_END_TOKEN:
            case YAML_FLOW_SEQUENCE_END_TOKEN:
            case YAML_FLOW_MAPPING_END_TOKEN:
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
print_preflight_count(TokenMark *token_mark, int level)
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
print_config( OlyStatus *status )
{
    OFILE *config_file = find_config_file(status);
    yaml_parser_t        config_parser ;
    yaml_token_t         token;
    TokenMark           *token_mark = preflight_config(status);

    zero_token_mark(token_mark);
    print_preflight_count(token_mark, 0);

    if ( *status != OLY_OKAY )
    {
        return;
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
                print_stdout_char_color(RED, BLACK, BRIGHT, 
                        "[Block END]\n");
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
                print_stdout_char_color(BLUE, BLACK, BRIGHT, "\n[Block Entry]");
                break;
            case YAML_FLOW_ENTRY_TOKEN:
                puts("[Flow Entry]");
                break;
            case YAML_KEY_TOKEN:
                token_mark->is_key_token = 1;
                break;
            case YAML_VALUE_TOKEN:
                token_mark->is_value_token = 1;
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
                if (token_mark->is_key_token == 1) 
                {
                    u_fprintf(u_stdout, "%s : ", token.data.scalar.value);
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
    return;
}

/* allocate a new token mark structure. */
static TokenMark *
new_token_mark (void)
{
    TokenMark *retval = (TokenMark *)omalloc(sizeof(TokenMark));
    retval->count_nodes = 0;
    retval->count_ochars = 0;
    retval->subnode = NULL ;
    zero_token_mark(retval);
    return retval;
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

