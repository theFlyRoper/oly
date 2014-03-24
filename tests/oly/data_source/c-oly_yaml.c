/* c-oly_yaml.c - Yaml language bindings for Oly.  License GPL2+ {{{
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
#include <yaml.h>
#include "tests/tap/basic.h"
#include "oly/core.h"
#include "oly/globals.h"
#include "src/data_source/oly_yaml.c"

/* MAIN */
int
main( int argc, char **argv )
{
    OlyStatus        status = OLY_OKAY;
    char            *locale = (char *)"root",  *charset = NULL;
    const char      *files[] = {
                        "tests/data/long_json.json",
                        "tests/data/every_token.yaml"
                     };
    int              record = 1;
    OlyDataSource   *ds = new_data_source( YAML_FILE , &status);

    if (set_data_filename( ds, files[record]) != OLY_OKAY)
    {
        printf("Could not load yaml file %s. Exiting...\n", files[record]);
        exit(EXIT_FAILURE);
    }
    oly             = init_oly( argv[0], TEST_PKGDATADIR, charset, locale );
    load_yaml( &status, ds );
    /*    print_yaml( &status , ds ); */
    printf("\n");

    exit(EXIT_SUCCESS);
}

