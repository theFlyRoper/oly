/* c-oly_timestamp - tests for oly_timestamp {{{
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

#include <assert.h>
#include <sys/stat.h>
#include <unicode/udat.h>
#include "oly/core.h"
#include "oly/globals.h"
#include "pvt_core.h"

#include "tests/tap/basic.h"
#include "tests/tap/float.h"
/* MAIN */
int
main( int argc, char **argv )
{
    const char *count_bytes_files[] = {
            "/data/orig_breaks.txt", "/data/spawner_output.txt",
            "/data/zh_romance_of_three_kingdoms.txt", ""},
            *source_dir = getenv("SOURCE");
    char    buffer[BUFSIZ], *locale = NULL, *charset = NULL;
    struct  stat sts;
    time_t  now;
    size_t  results[] = {4255, 142803, 46838, 0}, i = 0,
            output = 0;
    FILE    *f;
    UErrorCode status = U_ZERO_ERROR;
    int32_t string_length = 0;
    UChar*  test_string;
    if (source_dir == NULL)
    {
        printf("requires SOURCE environment variable, supplied by runtest. Exiting...");
        exit(EXIT_FAILURE);
    }
    oly = init_oly(argv[0], TEST_PKGDATADIR, charset, locale);
    plan(4);
    diag("testing oly_timestamp function.");
    time(&now);
    is_double((double now), oly_timestamp, 1000.0, "Two times should be close enough.");

    diag("testing count_file_bytes function.");
    for (i = 0; (results[i] != 0); i++) {
        strcpy(buffer, source_dir);
        strcat(buffer, count_bytes_files[i]);
        if (stat(buffer, &sts) == -1 && errno == ENOENT)
        {
            printf ("The file %s doesn't exist...\n", buffer);
        }
        f = fopen(buffer, "r");
        assert( count_file_bytes( f, &output ) == OLY_OKAY ) ;
        is_int(results[i], output, "File: %s", count_bytes_files[i]);
        fclose(f);
    }

    if ( 0 != 0 ) 
    {
        exit(EXIT_FAILURE);
    } 
    else 
    {
        exit(EXIT_SUCCESS);
    }
}

