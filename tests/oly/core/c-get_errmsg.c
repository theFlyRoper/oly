/* c-get_errmsg.c - tests for get_errmsg function.  License GPL2+ {{{
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

#include "tests/tap/basic.h"
#include "tests/tap/is_unicode_string.h"
#include "oly/common.h"
#include "oly/core.h"
#include "oly/globals.h"
#include "pvt_core.h"

/* MAIN */
int
main( int argc, char **argv ){
    const int       num_tests = ( 
            OLY_STATUS_MAX + OLY_STATUS_OFFSET );
    int              i = 0,
                     unknown_num = (OLY_ERR_UNKNOWN+OLY_STATUS_OFFSET);
    OChar          **results = (OChar **) xcalloc (
                                num_tests, sizeof(OChar));
        
    char            *locale = (char *)"root",  *charset = NULL;
    const char     *results_char[] = {
                        "OLY_WARN_REINIT\0",
                        "OLY_WARN_LIB\0",
                        "OLY_WARN_EXIT\0",
                        "OLY_OKAY\0",
                        "OLY_ERR_UNKNOWN\0",
                        "OLY_ERR_SYS\0",
                        "OLY_ERR_LIB\0",
                        "OLY_ERR_INIT\0",
                        "OLY_ERR_NOMEM\0",
                        "OLY_ERR_NOPWD\0",
                        "OLY_ERR_NOUSER\0",
                        "OLY_ERR_FILEIO\0",
                        "OLY_ERR_READHEX\0",
                        "OLY_ERR_HASH\0",
                        "OLY_ERR_BADARG\0",
                        "OLY_ERR_UNKNOWN\0"
                        };
    UErrorCode       u_status  = U_ZERO_ERROR;
    
    oly = init_oly(argv[0], TEST_PKGDATADIR, charset, locale);
    
    for ( i = 0; (i<=num_tests); i++)
    {
        results[i] = char_to_utf8(results_char[i]);
    }
    
    set_status(oly->state, OLY_OKAY);
    plan( num_tests + 12 );

    diag("From the smallest to the biggest error number.");
    for ( i = 0; (i<=num_tests); i++)
    {
        is_unicode_string(results[i], get_errmsg(i-OLY_STATUS_OFFSET), 
                "Item: %i, err: %S", 
                (i-OLY_STATUS_OFFSET), results[i]);
    }
    
    diag("One smaller than the smallest number.");
    is_unicode_string((results[unknown_num]), 
            get_errmsg(OLY_STATUS_MIN-1), "Number: %i",
            (i-OLY_STATUS_OFFSET));
    diag("A few wildly outlandish numbers.  Powers of -17!");
    for ( i = -17; (i<1000000); (i)*=(-17) )
    {
        is_unicode_string(
                results[(OLY_ERR_UNKNOWN+OLY_STATUS_OFFSET)],
                get_errmsg(i), "Number: %i", i);
    }
    
    diag("Letters and inappropriate hex digits YES MWAHAHAHAHAHA");
    i = 'a';
    is_unicode_string(results[(OLY_ERR_UNKNOWN+OLY_STATUS_OFFSET)],
            get_errmsg(i), "Number: %i, submitted: %c", i, i);
    i = '9';
    is_unicode_string(results[(OLY_ERR_UNKNOWN+OLY_STATUS_OFFSET)],
            get_errmsg(i), "Number: %i, submitted: %c", i, i);
    i = ' ';
    is_unicode_string(results[(OLY_ERR_UNKNOWN+OLY_STATUS_OFFSET)],
            get_errmsg(i), "Number: %i, submitted: %c (space)", i, i);
    i = 0xAAAA;
    is_unicode_string(results[(OLY_ERR_UNKNOWN+OLY_STATUS_OFFSET)],
            get_errmsg(i), "Number: %i, submitted: %c", i, i);
    i = 0xABBA;
    is_unicode_string(results[(OLY_ERR_UNKNOWN+OLY_STATUS_OFFSET)],
            get_errmsg(i), "AND ONE SWEDISH DANCING QUEEN (hex: %X)",
            i);
    if (U_FAILURE(u_status)) {
        exit(EXIT_FAILURE);
    } else {
        exit(EXIT_SUCCESS);
    }
}
