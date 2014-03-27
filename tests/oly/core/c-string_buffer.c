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
#include "oly/globals.h"
#include "oly/string_buffer.h"
#include "core/pvt_string_buffer.h"

#include "tests/tap/basic.h"
#include "tests/tap/float.h"

int
main( int argc, char **argv )
{
    OlyStatus status = OLY_OKAY;
    const char *count_bytes_files[] = {
            "/data/spawner_output.txt", ""},
            *source_dir = getenv("SOURCE");
    char     filebuffer[BUFSIZ],  *locale = "root", *charset = "UTF-8";
    OChar    in_buffer[BUFSIZ], out_buffer[BUFSIZ], *obuff;
    OFILE    *f;
    OlyStringBuffer *strbuf;
    size_t   size_read = 0, buffer_length = BUFSIZ, total_size_read = 0;
    size_t   outbuf_size = BUFSIZ, len_out = BUFSIZ;
    if (source_dir == NULL)
    {
        fprintf(stderr, "requires SOURCE environment variable, supplied by runtest. Exiting...\n");
        exit(EXIT_FAILURE);
    }
    oly = init_oly(argv[0], TEST_PKGDATADIR, charset, locale);

    status = open_string_buffer( &strbuf );
    HANDLE_STATUS_AND_DIE(status);
    strcpy(filebuffer, source_dir);
    strcat(filebuffer, count_bytes_files[0]);
    f = u_fopen(filebuffer, "rb", locale, charset);
    do {
        size_read = u_file_read(in_buffer, buffer_length, f);
        status = reserve_string_buffer( strbuf, buffer_length );
        status = enqueue_to_string_buffer(strbuf, (const OChar *)in_buffer, &size_read);

        if ((status != OLY_ERR_BUFFER_OVERFLOW) && (status != OLY_WARN_LONG_STRING))
        {
            HANDLE_STATUS_AND_DIE(status);
            total_size_read += size_read;
        }
        else
        {
            total_size_read += size_read;
            obuff = out_buffer;
            status = dequeue_from_string_buffer(strbuf, &obuff, outbuf_size, &len_out);
        }
    } while ((u_feof(f) != TRUE) && (status != OLY_ERR_BUFFER_OVERFLOW));
    
    HANDLE_STATUS_AND_DIE(status);

    exit(EXIT_SUCCESS);
}
 
