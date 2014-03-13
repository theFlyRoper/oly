/* c-count_file_bytes - tests for count_file_bytes. {{{
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
#include "oly/core.h"
#include "oly/oly_dev.h"
#include "tests/tap/basic.h"

static void close_main(void);

/* MAIN */
int
main( int argc, char **argv )
{
    const char *tests[] = {"/data/orig_breaks.txt",
            "/data/spawner_output.txt",
            "/data/zh_romance_of_three_kingdoms.txt", ""},
            *env = getenv("SOURCE");
    char    *c;
    struct stat sts;
    size_t  results[] = {4255, 142803, 46838, 0}, i = 0,
            output = 0;
    FILE    *f;
    Oly     *oly = (Oly *)xmalloc(sizeof(Oly));
    atexit (close_main);
    c = (char *)xmalloc(BUFSIZ);
    memset((void *)c, '\0', BUFSIZ);

    plan(3);
    for (i = 0; (results[i] != 0); i++) {
        strcpy(c, env);
        strcat(c, tests[i]);
        if (stat(c, &sts) == -1 && errno == ENOENT)
        {
            printf ("The file %s doesn't exist...\n", c);
        }
        f = fopen(c, "r");
        assert( count_file_bytes(f, &output, oly) == OLY_OKAY ) ;
        is_int(results[i], output, "File: %s", tests[i]);
        fclose(f);
    }

    free(c);
    if ( 0 != 0 ) 
    {
        exit(EXIT_FAILURE);
    } 
    else 
    {
        exit(EXIT_SUCCESS);
    }
}

static void 
close_main (void) 
{
    if (fclose (stdout) != 0) {
        perror ("resex: write error");
        exit (EXIT_FAILURE);
    }
    if (fclose (stdin) != 0) {
        perror ("resex: read error on close");
        exit (EXIT_FAILURE);
    }
}

