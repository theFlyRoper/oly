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

struct oly_boundary_struct;
typedef struct oly_boundary_struct OlyBoundary;

struct oly_boundary_struct
{
    OChar           *o_now;
    OChar           *o_end;
    OChar           *o_start;
    OChar           *o_flush_break;
    char            *c_now;
    char            *c_end;
    char            *c_start;
    char            *c_flush_break;
    UConverter      *converter;
};

extern YAMLOly *open_yaml_oly( const char *file, const char *charset, 
        const char *locale, OlyStatus *status );
extern void close_yaml_oly( YAMLOly *close_me );

extern SQLiteOly *open_sqlite_oly( const char *file, const char *charset, 
        const char *locale, OlyStatus *status );
extern void close_sqlite_oly( SQLiteOly *close_me );
extern OlyStatus set_sqlite_sql( SQLiteOly *db, char *sql);
extern OlyBoundary *open_oly_boundary(OlyDataSource *ds, OlyStatus *status);

/* Goal here is to figure out the order things
 * must follow so that ICU and other data sources can
 * work together effectively. */
int
main( int argc, char **argv ) 
{
    OlyStatus        status = OLY_OKAY;
    char             name_buffer[BUFSIZ];
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
    yaml_data       = open_yaml_oly((const char *)name_buffer, (const char *)charset, 
                        (const char *)locale, &status );

    if (status != OLY_OKAY)
    {
        pick_up_phone_booth_and_die(status);
    }
    
    strcpy(name_buffer, DATASOURCEDIR); 
    strcat(name_buffer, sqlite_file);
    sqlite_data     = open_sqlite_oly((const char *)name_buffer, (const char *)charset, 
                        (const char *)locale, &status );
    
    if (status != OLY_OKAY)
    {
        pick_up_phone_booth_and_die(status);
    }
/* (sqlite_data == NULL) || (yaml_data == NULL) || */
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

OlyBoundary *
open_oly_boundary(OlyDataSource *ds, OlyStatus *status)
{
    UErrorCode   u_status = U_ZERO_ERROR;
    size_t       o_size = 0, c_size = 0, max_characters = 0, min_char_size = 0,
                 buffer_max_size = get_max_buffer_size(ds, status);
    char        *break_ptr;
    OlyBoundary *oly_bound = omalloc(sizeof(OlyBoundary));
    /* oly boundary uses two buffers:
     * 1. Buffer full of input characters
     * 2. Buffer full of OChars 
     *
     * to use it, drop chars in the input buffer and once it is full it will flush
     * them.
     */

    /* Allocate the space for both buffers,
     * then chop it into two spaces according to character size ratios. 
     * Add sizeof(OChar) to accomodate an OChar sized null space to 
     * delineate the two areas. 
     * Consertative and wastes a bit of the end of the buffer.
     * Better that then overflows.
     */
    oly_bound->converter   = ucnv_open( get_data_charset(ds), &u_status );
    min_char_size = ucnv_getMinCharSize(oly_bound->converter) ;
    max_characters      = (buffer_max_size / ( min_char_size + sizeof(OChar)));
    o_size          = (( max_characters * sizeof(OChar) ) - sizeof(OChar));
    c_size          = (( max_characters * min_char_size ) - min_char_size);
    DLOG("\n\to_size: %llu, c_size: %llu, max_buffer_size: %llu\n", 
            (unsigned long long)o_size, (unsigned long long)c_size, 
            (unsigned long long)buffer_max_size);

    /* normally the ICU converter API only uses 6 pointers, 
     * 3 for the outside charset (c_) and 3 for the ICU internal
     * UChar structures (o_).  The flush_break pointers are 
     * there to hold the spot for higher nodes.
     */

    oly_bound->c_now            = (char *)omalloc( o_size + c_size 
            + min_char_size + sizeof(OChar) );
    oly_bound->c_start          = oly_bound->c_now;
    /* because the char buffers are actually pointing to different 
     * character sets, although we only expect to find max_characters characters in it,
     * as they are char pointers we must move it the correct number of chars. */
    oly_bound->c_end            = ((oly_bound->c_now + ((max_characters*min_char_size)- min_char_size*2)));
    oly_bound->c_flush_break    = oly_bound->c_now;

    /* OChar buffer */
    oly_bound->o_now            = (OChar *)((oly_bound->c_end) + min_char_size + 1);
    oly_bound->o_start          = oly_bound->o_now;
    /* unlike the char buffer, the OChar buffer points to its actual character set.
     * Thus, it must only move max_characters forward, as opposed to 
     * o_size. */
    oly_bound->o_end            = ((oly_bound->o_now) + (max_characters-2));
    oly_bound->o_flush_break    = oly_bound->o_now;
    

    
    /* make sure the area after each buffer is null. */
    for ( break_ptr = ((oly_bound->c_end) + 1); 
            ( break_ptr < (char *)oly_bound->o_now ); break_ptr++ )
    {
        *break_ptr = '\0' ;
    }
    
    for ( break_ptr = (char *)(oly_bound->o_end); 
            ( break_ptr < (oly_bound->c_start + buffer_max_size )); break_ptr++ )
    {
        *break_ptr = '\0' ;
    }
    DLOG("\nc_start = 1.\n With that context, here are relative positions:"
            "\nc_end: %llu, o_start: %llu, o_end %llu\n", 
        (unsigned long long)(oly_bound->c_end - oly_bound->c_start),
        (unsigned long long)((char *)oly_bound->o_start - oly_bound->c_start),
        (unsigned long long)((char *)oly_bound->o_end - oly_bound->c_start)
        );
    o_size = (size_t)(oly_bound->o_end - oly_bound->o_start);
    c_size = (size_t)((oly_bound->c_end - oly_bound->c_start)/min_char_size);
    DLOG("\nnumber of ochars: %zu,  number of chars: %zu\n", 
        o_size, c_size);
    
    assert( o_size == c_size );

    o_size = (size_t)((char *)oly_bound->o_end - (char *)oly_bound->o_start);
    c_size = (size_t)(oly_bound->c_end - oly_bound->c_start);
    assert( ( o_size + c_size ) < buffer_max_size );
    assert( ( o_size % sizeof(OChar) ) == 0 );
    assert( ( c_size % min_char_size ) == 0 );
    break_ptr = oly_bound->c_start;
    assert(break_ptr[1023] == '\0' );
    
    assert( &break_ptr[1023] == (break_ptr + 1023));

    return oly_bound;
};


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

SQLiteOly *open_sqlite_oly( const char *file, const char *charset, const char *locale, 
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
    free((void *)close_me->raw_sql);
}

OlyStatus
set_sqlite_sql( SQLiteOly *db, char *sql)
{
    db->raw_sql = sql;
    int len = (strlen(sql)+1);
    sqlite3_stmt *prepped = db->prepped ;
    OlyStatus status = OLY_OKAY;
    if ( sqlite3_prepare_v2(db->data, sql, len, &prepped, NULL ) != SQLITE_OK )
    {
        status = OLY_ERR_SQLITE;
        return status;
    }

    db->prepped = prepped;
    return status;
}

OlyStatus
get_options(int argc, char **argv)
{
    OlyStatus status = OLY_OKAY;
    char c;
    /* y = YAML file, q = sqlite file, h = help, v = version */
    while ((c = getopt(argc, argv, "y:q:hv")) != -1) 
    {
        switch (c) 
        {
        case ('h'):
            status = OLY_WARN_SHOW_HELP;
            break;
        case ('v'):
            status = OLY_WARN_SHOW_VERSION;
            break;
        case ('y'):
            yaml_file = optarg;
            break;
        case ('q'):
            sqlite_file = optarg;
            break;
        default:
            status = OLY_ERR_BADARG;
            break;
        }
    }
    return status;
}

static void
pick_up_phone_booth_and_die( OlyStatus status )
{
            u_fprintf(u_stderr, "ERROR: ");
            u_fprintf_u(u_stderr, get_errmsg(status));
            u_fprintf(u_stderr, " Exiting...\n");
            exit(EXIT_FAILURE);
}

static void 
print_help(void)
{
    usage();
    u_fprintf(u_stdout,"A prototype to test buffering approaches between a \ncollection data source (YAML) and a table data source (sqlite 3).\n");
    u_fprintf(u_stdout,"Options:\n");
    u_fprintf(u_stdout,"\t\t-h\t\tShow this help message.\n");
    u_fprintf(u_stdout,"\t\t-v\t\tShow the program version.\n");
    u_fprintf(u_stdout,"\t\t-y\t\tyaml file to use.\n");
    u_fprintf(u_stdout,"\t\t-q\t\tsqlite file to use.\n");
    exit(EXIT_SUCCESS);
}

static void 
usage(void){
    u_fprintf(u_stdout, "%S: [OPTIONS]\n", 
           get_program_name());
}

static void 
print_version(void){
    u_fprintf(u_stdout, "%S: Version 1.0, 3-21-2014 - copyright (C) Oly Project\n", 
           get_program_name());
    u_fprintf(u_stdout, "\tLicensed according to the terms of the GNU general public\n");
    u_fprintf(u_stdout, "\tlicense, version 2 or (at your option) any later version.\n");
    u_fprintf(u_stdout, "\tThis program is provided in the hopes that it will be useful, but\n");
    u_fprintf(u_stdout, "\tWITHOUT ANY WARRANTY, to the extent permitted by law.\n\n");
    u_fprintf(u_stdout, "\tVisit %s for precise details on authorship.\n", PACKAGE_URL);
}


