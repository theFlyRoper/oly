/* yaml_to_sqlite.c - prototype for moving data from yaml documents to sqlite.  License GPL2+ {{{
 * Copyright (C) 2014 Oly Project
 * Permission is hereby granted, free of charge, to any person obtaining a copy of 
 * this software and associated documentation files (the "Software"), to deal in 
 * the Software without restriction, including without limitation the rights to 
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
 * of the Software, and to permit persons to whom the Software is furnished to do 
 * so, subject to the following conditions:
 * 
 *  The above copyright notice and this permission notice shall be included in 
 *  all copies or substantial portions of the Software.
 *  
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 * }}} */

#include "oly/common.h"
#include "oly/data_source.h"
#include "oly/node.h"
#include "oly/core.h"
#include "oly/globals.h"


#ifdef HAVE_SQLITE3
#include <sqlite3.h>
#endif /* HAVE_SQLITE3 */

#ifdef HAVE_YAML_H
#include <yaml.h>
#endif /* HAVE_YAML_H */

#ifdef OLYDEV
    #define DLOG(fmt, args...) printf("%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__,args)
#else
    #define DLOG(fmt, args...)
#endif

static void print_help(void);
static void usage(void);
static void print_version(void);
static void pick_up_phone_booth_and_die( OlyStatus status );
static char *locdir = PKGDATADIR, *locale = "root", *charset = "UTF-8", 
    *yaml_file = "/tests/data/every_token.yaml", 
    *sqlite_file = "/tests/data/test_sqlite.sqlite3";

static void yaml_input_ofile(yaml_parser_t *parser, OFILE *file);
static int ofile_read_handler(void *data, 
        unsigned char *buffer, size_t size, size_t *size_read);

OlyStatus get_options( int argc, char **argv );

typedef struct sqlite_oly_struct {
    char            *file_name;
    char            *charset;
    char            *locale;
    sqlite3         *data;
    sqlite3_stmt    *prepped;
    char            *raw_sql;
} SQLiteOly;

typedef struct yaml_oly_struct {
    char    *file_name;
    char    *locale;
    char    *charset;
    OFILE   *data;
    yaml_parser_t *parser ;
    yaml_token_t  *token;
} YAMLOly;

extern OlyStatus delete_file(char *file);

extern YAMLOly *open_yaml_oly( const char *file, const char *charset, 
        const char *locale, OlyStatus *status );
extern void close_yaml_oly( YAMLOly *close_me );

extern SQLiteOly *open_sqlite_oly( const char *file, const char *charset, 
        const char *locale, OlyStatus *status );
extern void close_sqlite_oly( SQLiteOly *close_me );
extern OlyStatus set_sqlite_sql( SQLiteOly *db, char *sql);
extern OlyStatus exec_sqlite_sql( SQLiteOly *db );
extern OlyStatus finalize_sqlite_sql( SQLiteOly *db );

/* Goal here is to figure out the order things
 * must follow so that ICU and other data sources can
 * work together effectively. */

#include "tests/prototypes/incl-yaml_to_sqlite.c"
int
main( int argc, char **argv ) 
{
    OlyStatus        status = OLY_OKAY;
    char             name_buffer[BUFSIZ];
    char            *create_table_node = "create table node (id integer primary key,"
                        "\nkey text, value text);\n"
                        "create table node_relation (\n"
                        "id integer primary key,\n"
                        "node_id integer not null, parent_node_id integer not null,\n"
                        "foreign key (node_id) references node(id),\n"
                        "foreign key (parent_node_id) references node(id)"
                        "unique (node_id, parent_node_id));";
    OlyBoundary     *oly_bound;
    YAMLOly         *yaml_data;
    SQLiteOly       *sqlite_data;
    OlyDataSource   *ds;
    size_t           buffer_size = 1024;

    oly     = init_oly(argv[0], locdir, charset, locale);
    status  = get_options(argc, argv);

    switch (status)
    {
        case OLY_WARN_SHOW_VERSION:
            print_version();
            status = OLY_OKAY;
            exit(EXIT_SUCCESS);
        case OLY_WARN_SHOW_HELP:
            print_help();
            status = OLY_OKAY;
            exit(EXIT_SUCCESS);
        case OLY_OKAY:
            break;
        default:
            usage();
            pick_up_phone_booth_and_die(status);
    }

    ds          = new_data_source( YAML_FILE , &status );
    set_max_buffer_size( ds, buffer_size );
    set_data_charset( ds, charset );
    oly_bound   = open_oly_boundary(ds, &status);
    
    if (oly_bound == NULL)
    {
        pick_up_phone_booth_and_die(status);
    }

    strcpy(name_buffer, DATASOURCEDIR); 
    strcat(name_buffer, yaml_file);
    yaml_data = open_yaml_oly((const char *)name_buffer, (const char *)charset, 
                        (const char *)locale, &status );
    
    if (status != OLY_OKAY)
    {
        pick_up_phone_booth_and_die(status);
    }
    
    strcpy(name_buffer, DATASOURCEDIR); 
    strcat(name_buffer, sqlite_file);
    delete_file(name_buffer);
    sqlite_data = open_sqlite_oly((const char *)name_buffer, (const char *)charset, 
                        (const char *)locale, &status );
    
    if (status != OLY_OKAY)
    {
        pick_up_phone_booth_and_die(status);
    }
   
    /* here we create the node and node_relation table */
    set_sqlite_sql( sqlite_data, create_table_node );
    exec_sqlite_sql( sqlite_data );
    finalize_sqlite_sql( sqlite_data );
    printf("built the node and node_relation tables.\n");
    

    close_yaml_oly( yaml_data );
    close_sqlite_oly( sqlite_data );

    if (status == OLY_OKAY)
    {
        exit(EXIT_SUCCESS);
    }
    else
    {
        pick_up_phone_booth_and_die(status);
    }
}

YAMLOly *
open_yaml_oly( const char *file, const char *charset, const char *locale, 
        OlyStatus *status )
{

    YAMLOly     *yaml_data = (YAMLOly *)omalloc(sizeof(YAMLOly));

    yaml_data->parser = omalloc(sizeof(yaml_parser_t));
    yaml_data->token = omalloc(sizeof(yaml_token_t));
    yaml_data->file_name = (char *)file;
    yaml_data->charset = (char *)charset;
    yaml_data->locale = (char *)locale;
    yaml_data->data = (OFILE *)u_fopen( file, "rd", locale, charset ); 
    if ( yaml_data->data == NULL )
    {
        *status = OLY_ERR_FILE_NOT_FOUND ;
        return NULL;
    }
    if ( yaml_parser_initialize( (yaml_data->parser) ) != 1 )
    {
        *status = OLY_ERR_LIBYAML_INIT;
        return NULL;
    };
    yaml_parser_set_encoding( yaml_data->parser, YAML_UTF16LE_ENCODING );
    yaml_input_ofile( yaml_data->parser, yaml_data->data );
    return yaml_data;
}

void 
close_yaml_oly( YAMLOly *close_me )
{
    free((void *)close_me->parser);
    free((void *)close_me->token);
    u_fclose(close_me->data);
    free((void *)close_me);
}

OlyStatus
yaml_to_nodes(YAMLOly *ds, OlyDataSource *data)
{
    char ultrabuffer[BUFSIZ];
    OlyStatus status = OLY_OKAY;
    do {
        if (!yaml_parser_scan(ds->parser, ds->token)) 
        {
            printf("Parser error %d\n", ds->parser->error);
            exit(EXIT_FAILURE);
        }
        switch(ds->token->type)
        { 
            case YAML_BLOCK_SEQUENCE_START_TOKEN:
            case YAML_FLOW_SEQUENCE_START_TOKEN:
                break;
            case YAML_BLOCK_MAPPING_START_TOKEN:
            case YAML_FLOW_MAPPING_START_TOKEN:
                break;
            case YAML_FLOW_MAPPING_END_TOKEN:
            case YAML_FLOW_SEQUENCE_END_TOKEN:
            case YAML_BLOCK_END_TOKEN:
                break;
            case YAML_BLOCK_ENTRY_TOKEN:
            case YAML_FLOW_ENTRY_TOKEN:
            case YAML_KEY_TOKEN:
                break;
            case YAML_VALUE_TOKEN:
                break;
            case YAML_ALIAS_TOKEN:
                sprintf(ultrabuffer, "%s\n", ds->token->data.alias.value);
                break;
            case YAML_ANCHOR_TOKEN:
                sprintf(ultrabuffer, "\n[Anchor Token] %s\n", 
                        ds->token->data.anchor.value);
                break;
            case YAML_TAG_TOKEN:
                sprintf(ultrabuffer, "\n[Tag Token] handle: %s, suffix: %s\n", ds->token->data.tag.handle, ds->token->data.tag.suffix);
                break;
            case YAML_SCALAR_TOKEN:
                u_fprintf(u_stdout, "%s : ", ds->token->data.scalar.value);
                break;
            case YAML_STREAM_START_TOKEN:
            case YAML_STREAM_END_TOKEN:
            case YAML_VERSION_DIRECTIVE_TOKEN:
            case YAML_TAG_DIRECTIVE_TOKEN:
            case YAML_DOCUMENT_START_TOKEN:
            case YAML_DOCUMENT_END_TOKEN:
            default: 
                break;
        }
        if(ds->token->type != YAML_STREAM_END_TOKEN)
        {
            yaml_token_delete(ds->token);
        }
    } while(ds->token->type != YAML_STREAM_END_TOKEN) ;
    return status;
}

SQLiteOly *
open_sqlite_oly( const char *file, const char *charset, const char *locale, 
        OlyStatus *status )
{
    SQLiteOly       *sqlite_data = (SQLiteOly *)omalloc(sizeof(SQLiteOly));
    sqlite3         *database ;
   
    sqlite_data->file_name  = (char *)file;
    sqlite_data->charset    = (char *)charset;
    sqlite_data->locale     = (char *)locale;
    if ( sqlite3_open(file, &database) != SQLITE_OK )
    {
        printf("sqlite file: %s\n", file);
        *status = OLY_ERR_SQLITE_INIT;
        return NULL;
    }
    sqlite_data->data = database;
    return sqlite_data;
}

void 
close_sqlite_oly( SQLiteOly *close_me )
{
    if (close_me->prepped != NULL)
    {
        sqlite3_finalize( close_me->prepped );
    }
    sqlite3_close(close_me->data);
}

OlyStatus
set_sqlite_sql( SQLiteOly *db, char *sql)
{
    int len = (strlen(sql)+1);
    const char *unused;
    db->raw_sql = sql;
    sqlite3_stmt *prepped = NULL;
    OlyStatus status = OLY_OKAY;
    if ( sqlite3_prepare_v2( db->data, sql, len, &prepped, &unused ) != SQLITE_OK )
    {
        printf("SQLite err: %s\n", sqlite3_errmsg( db->data ));
        status = OLY_ERR_SQLITE;
        return status;
    }

    db->prepped = prepped;
    return status;
}

OlyStatus
exec_sqlite_sql( SQLiteOly *db )
{
    OlyStatus status = OLY_OKAY;
    int sqlite_return = sqlite3_step( db->prepped );
    if (sqlite_return != SQLITE_DONE)
    {
        printf("SQLite err: %s\n", sqlite3_errmsg( db->data ));
        status = OLY_ERR_SQLITE ;
    }
    return status;
}

OlyStatus
finalize_sqlite_sql( SQLiteOly *db )
{
    OlyStatus status = OLY_OKAY;
    if (db->prepped != NULL)
    {
        sqlite3_finalize( db->prepped );
    }
    return status;
}

OlyStatus
delete_file(char *file)
{
    OlyStatus status = OLY_OKAY;
    if (unlink( file) != 0)
    {
        status = OLY_ERR_FILEIO;
        printf("delete file %s err: %s\n", file, ostrerror(errno));
    }
    return status;
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
