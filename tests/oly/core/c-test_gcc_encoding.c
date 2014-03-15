/* c-test_gcc_encoding.c - test how GCC deals with encoding string literals. License GPL2+ 
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
 */

#include "oly/common.h"
#include "oly/core.h"
#include "oly/globals.h"

/* MAIN */
int
main( int argc, char **argv ){
    const int       num_tests = ( 
            OLY_STATUS_MAX + OLY_STATUS_OFFSET + 1 );
    int              val = 0;
    char            *locale = (char *)"root",  *charset = "cp-1252";

    oly = init_oly(argv[0], TEST_PKGDATADIR, charset, locale);

    u_fprintf_u(u_stdout, get_errmsg(OLY_OKAY));
    u_fprintf_u(u_stdout, u"\nDefault IO locale: %S\n",
                get_default_locale() );
    if ((val = u_strcmp(get_errmsg(OLY_OKAY), u"OLY_OKAY")) == 0)
    {
        u_fprintf_u(u_stdout, u"L u_strcmp agrees!\n");
    }
    else
    {
        u_fprintf_u(u_stdout, u"L u_strcmp not agrees :(\n");
    };
    u_fprintf(u_stdout, "L u_strcmp diff: %i\n", val);
    

    exit(EXIT_SUCCESS);
}

