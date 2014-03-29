/* c-string_buffer - tests for functions in src/core/string_buffer.c {{{
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

#include <sys/stat.h>
#include <assert.h>
#include "oly/core.h"
#include "oly/oly_dev.h"
#include "oly/globals.h"
#include "oly/string_buffer.h"
#include "core/pvt_string_buffer.h"

#include "tests/tap/basic.h"
U_STRING_DECL(empty, "", 1); 
/*
static void enqueue_until_full(OlyStringBuffer *strbuf, OFILE *f);
static void dequeue_until_empty(OlyStringBuffer *strbuf, OFILE *f);
static void enqueue_rows(OlyStringBuffer *strbuf, OFILE *f, size_t rows, size_t *rows_enqueued);
static void dequeue_rows(OlyStringBuffer *strbuf, OFILE *f, size_t rows, size_t *rows_dequeued);
*/
int
main( int argc, char **argv )
{
    const size_t  buffer_length = 256;
    OlyStatus status = OLY_OKAY;
    const char *count_bytes_files[] = {
        "/data/UTF-8-demo.txt", "/data/realistic_data.txt", ""
    },
            *source_dir = getenv("SOURCE");
    char     filebuffer[buffer_length],  *locale = "root", *charset = "UTF-8";
    OChar    in_buffer[buffer_length], out_buffer[buffer_length], *obuff,
             **wanted;
    OFILE    *f;
    OlyStringBuffer *strbuf;
    size_t   size_read = 0, total_size_read = 0, rows_read = 0;
    size_t   outbuf_size = buffer_length, len_out = buffer_length;
    U_STRING_INIT(empty, "", 1); 
    
    if (source_dir == NULL)
    {
        fprintf(stderr, "requires SOURCE environment variable, supplied by runtest. Exiting...\n");
        exit(EXIT_FAILURE);
    }
    oly = init_oly(argv[0], TEST_PKGDATADIR, charset, locale);
    
    strcpy(filebuffer, source_dir);
    strcat(filebuffer, count_bytes_files[0]);
    f = u_fopen(filebuffer, "rb", locale, charset);
    if( f == NULL )
    {
        sysbail("test file %s not found!", filebuffer);
    }
    while (u_feof(f) != TRUE)
    {
        u_fgets(in_buffer, buffer_length, f);
        rows_read++;
        size_read += (u_strlen(in_buffer)+3); 
    };
    wanted = (OChar **)ocalloc(((rows_read * sizeof(OChar *)) 
                + (size_read * sizeof(OChar))), sizeof(char));
    obuff = (OChar *)(wanted + rows_read + 1);
    if( wanted == NULL )
    {
        bail("Could not map test file!");
    }
    rows_read = 0;
    size_read = 0;
    u_frewind(f);
    while (u_feof(f) != TRUE)
    {
        u_fgets(in_buffer, buffer_length, f);
        wanted[rows_read++] = obuff;
        u_strcpy(obuff, in_buffer);
        obuff += (u_strlen(in_buffer)+2);
    };
    u_frewind(f);
    plan( rows_read + 20 );
    diag("---- tests for OlyStringBuffer data structure. ----");
    
    diag("OlyStringBuffer depends on function get_main_string_buffer_max.  It should return 1024 for these tests, because these tests were designed at that number.");
    if( 1024 != get_main_string_buffer_max())
    {
        bail("get_main_string_buffer_max: it should return 1024.");
    }
    status = open_string_buffer( &strbuf );
    is_int(status, OLY_OKAY, "confirm open_string_buffer returns OLY_OKAY");

    size_read = 0; 
    rows_read = -1; 
    diag("File test: UTF-8-demo.txt (thx, Markus Kuhn! <3 <3)");
    obuff = out_buffer;
    while (u_feof(f) != TRUE)
    {
        u_fgets(in_buffer, buffer_length, f);
        status = reserve_string_buffer( strbuf, buffer_length );
        if ((status == OLY_ERR_BUFFER_OVERFLOW) || (status == OLY_WARN_LONG_STRING))
        {
            while (status != OLY_WARN_BUFFER_EMPTY)
            {
                rows_read++;
                status = dequeue_from_string_buffer(strbuf, &obuff, outbuf_size, &len_out);
                if (status != OLY_WARN_BUFFER_EMPTY)
                {
                    is_unicode_string(wanted[rows_read], obuff, 
                            "Row: %llu", (long long unsigned)rows_read);
                }
            }
            rows_read--;
            status = OLY_OKAY;
            status = reserve_string_buffer( strbuf, buffer_length );
        }
        total_size_read += size_read;
        status = enqueue_to_string_buffer(strbuf, (const OChar *)in_buffer, &size_read);
    };
    
    while (status != OLY_WARN_BUFFER_EMPTY)
    {
        rows_read++;
        status = dequeue_from_string_buffer(strbuf, &obuff, outbuf_size, &len_out);
        if (status != OLY_WARN_BUFFER_EMPTY)
        {
            is_unicode_string(wanted[rows_read], obuff, 
                    "Row: %llu", (long long unsigned)rows_read);
        }
    }
    
    diag("Status and boundary tests, reserve and enqueue.");
    status = reserve_string_buffer( strbuf, buffer_length );
    is_int(OLY_OKAY, status, "Check results for reserve.");
    status = reserve_string_buffer( strbuf, buffer_length );
    is_int(OLY_WARN_BUFFER_WRITE_LOCK, status, "Gives OLY_WARN_BUFFER_WRITE_LOCK if enqueue has not been called after reserve.");
    status = enqueue_to_string_buffer(strbuf, empty, &size_read);
    is_int(status, OLY_OKAY, "Enqueue to string buffer status for empty string.");
    is_int(0, size_read, "Empty string.");
    status = reserve_string_buffer( strbuf, (get_main_string_buffer_max()*2) );
    is_int(OLY_ERR_BUFFER_OVERFLOW, status, "Try to reserve more than is possible.");
    status = OLY_OKAY;
   
    /* I am not sure how to test this.  It does not set strbuf to null, it just frees
     * its memory and sets its pointers to null.*/
    close_string_buffer( strbuf );
        
    is_int(7778, total_size_read, "total read: %zu\n", total_size_read);
    HANDLE_STATUS_AND_DIE(status);

    exit(EXIT_SUCCESS);
}
/*
static void 
enqueue_until_full(OlyStringBuffer *strbuf, OFILE *f)
{
}
    
static void 
dequeue_until_empty(OlyStringBuffer *strbuf, OFILE *f)
{
}
    
static void 
enqueue_rows(OlyStringBuffer *strbuf, OFILE *f, size_t rows, size_t *rows_enqueued)
{
}

static void 
dequeue_rows(OlyStringBuffer *strbuf, OFILE *f, size_t rows, size_t *rows_dequeued)
{
}
*/

