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

int
main( int argc, char **argv )
{
    const size_t  buffer_length = 256;
    OlyStatus status = OLY_OKAY;
    const char *count_bytes_files[] = {
        "/data/orig_breaks.txt", "/data/realistic_data.txt", ""
    },
            *source_dir = getenv("SOURCE");
    char     filebuffer[buffer_length],  *locale = "root", *charset = "UTF-8";
    OChar    in_buffer[buffer_length], out_buffer[buffer_length], *obuff;
    OFILE    *f;
    OlyStringBuffer *strbuf;
    size_t   size_read = 0, total_size_read = 0;
    size_t   outbuf_size = buffer_length, len_out = buffer_length;
    
    plan( 20 );
    if (source_dir == NULL)
    {
        fprintf(stderr, "requires SOURCE environment variable, supplied by runtest. Exiting...\n");
        exit(EXIT_FAILURE);
    }
    oly = init_oly(argv[0], TEST_PKGDATADIR, charset, locale);
    obuff = out_buffer;

    status = open_string_buffer( &strbuf );
    HANDLE_STATUS_AND_DIE(status);
    strcpy(filebuffer, source_dir);
    strcat(filebuffer, count_bytes_files[1]);
    f = u_fopen(filebuffer, "rb", locale, charset);
    while (u_feof(f) != TRUE)
    {
        u_fgets(in_buffer, buffer_length, f);
        status = reserve_string_buffer( strbuf, buffer_length );

        if ((status == OLY_ERR_BUFFER_OVERFLOW) || (status == OLY_WARN_LONG_STRING))
        {
            while (status != OLY_WARN_BUFFER_EMPTY)
            {
                status = dequeue_from_string_buffer(strbuf, &obuff, outbuf_size, &len_out);
                u_fprintf_u(u_stdout, obuff);
            }
            status = OLY_OKAY;
        }
        else
        {
            total_size_read += size_read;
            status = enqueue_to_string_buffer(strbuf, (const OChar *)in_buffer,
                    &size_read);
        }
    } ;
    
    while (status != OLY_WARN_BUFFER_EMPTY)
    {
        status = dequeue_from_string_buffer(strbuf, &obuff, outbuf_size, &len_out);
        u_fprintf_u(u_stdout, obuff);
    }
        
    printf("total read: %zu, output length: %i\n", total_size_read, u_strlen(obuff));
    status = OLY_OKAY;
    HANDLE_STATUS_AND_DIE(status);

    exit(EXIT_SUCCESS);
}
 
