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

#ifdef HAVE_SQLITE3
#include <sqlite3.h>
#endif /* HAVE_SQLITE3 */

#ifdef HAVE_YAML_H
#include <yaml.h>
#endif /* HAVE_YAML_H */

#include "oly/olytypes.h"
#undef DEFAULT_BUFFER_SIZE
#define DEFAULT_BUFFER_SIZE 1024
#include "oly/data_source.h"
#include "oly/node.h"
#include "oly/core.h"
#include "oly/globals.h"
#include "pvt_data_source.h"

#include "node/pvt_node.h"

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
    yaml_event_t  *event;
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
    YAMLOly         *yaml_data;
    SQLiteOly       *sqlite_data;
    OlyDataSource   *yaml_ds;
    OlyDataSource   *sqlite_ds;
    size_t           buffer_size = DEFAULT_BUFFER_SIZE;

    status = init_oly(argv[0], locdir, charset, locale, &oly );
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

    yaml_ds = new_data_source( YAML_FILE , &status );
    sqlite_ds = new_data_source( SQLITE_FILE , &status );
    set_max_buffer_size( yaml_ds, buffer_size );
    set_data_charset( yaml_ds, charset );
    set_max_buffer_size( sqlite_ds, buffer_size );
    set_data_charset( sqlite_ds, charset );

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
    yaml_data->event = omalloc(sizeof(yaml_event_t));
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

typedef enum oly_yaml_status_enum {
    OLY_YAML_OKAY,
    OLY_YAML_SEQUENCE,
    OLY_YAML_MAP,
    OLY_YAML_VALUE
} OlyYAMLStatus;

OlyStatus
yaml_to_nodes(YAMLOly *ds, OlyDataSource *data)
{
    OlyStatus           status      = OLY_OKAY;
    OlyYAMLStatus       yaml_status = OLY_YAML_OKAY;
    unsigned char       have_key    = 0x0;
    do {
        if (!yaml_parser_parse(ds->parser, ds->event))
        {
            printf("Parser error %d\n", ds->parser->error);
            exit(EXIT_FAILURE);
        }
        switch(ds->event->type)
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
                yaml_status = OLY_YAML_SEQUENCE;
                status = push_ds_node( data, OLY_NODE_VALUE_TYPE_SEQUENCE );
                have_key = 0x0;
                break;
            case YAML_SEQUENCE_END_EVENT:
                status = pop_ds_node( data, OLY_NODE_VALUE_TYPE_SEQUENCE );
                yaml_status = OLY_YAML_OKAY;
                break;
            case YAML_MAPPING_START_EVENT:  
                status = push_ds_node( data, OLY_NODE_VALUE_TYPE_MAP );
                have_key = 0x0;
                break;
            case YAML_MAPPING_END_EVENT:    
                yaml_status = OLY_YAML_OKAY;
                status = pop_ds_node( data, OLY_NODE_VALUE_TYPE_MAP );
                break;
            case YAML_ALIAS_EVENT:  
                break;
            case YAML_SCALAR_EVENT:
                if ( ( have_key == 0x0 ) && ( yaml_status != OLY_YAML_SEQUENCE ) )
                {
                    status = stage_node_key( data, 
                        (const char *)ds->event->data.scalar.value );
                    have_key = 0x1;
                }
                else 
                {
                    if (enqueue_ds_node( data, ds->event->data.scalar.value,
                            OLY_NODE_VALUE_SCALAR_STRING) != OLY_OKAY)
                    {
                        HANDLE_STATUS_AND_RETURN( data->status );
                    };
                    have_key = 0x0;
                }
                break;
            case YAML_NO_EVENT: 
                break;
            default: 
                break;
        }
        if( ds->event->type != YAML_STREAM_END_EVENT )
        {
            yaml_event_delete(ds->event);
        }
    } while( ds->event->type != YAML_STREAM_END_EVENT ) ;
    return status;
}

void
yaml_next(YAMLOly *ds)
{
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
