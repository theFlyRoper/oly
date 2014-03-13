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
static void close_main(void);

/* MAIN */
int
main( int argc, char **argv )
{
    
    UErrorCode status = U_ZERO_ERROR;
    int32_t i, myStrlen = 0;
    UChar* myString;
    char buffer[1024], *locale = NULL, *charset = NULL,
                        *locdir=(char*)TEST_PKGDATADIR;
    UDate myDateArr[] = { 0.0, 10000000000.0, 200000000000.0, oly_timestamp() }; 
    UDateFormat* df = udat_open(UDAT_DEFAULT, UDAT_DEFAULT, NULL, NULL, -1, NULL, 0, &status);
    atexit (close_main);
    oly = init_oly(argv[0], locdir, NULL, locale);
    printf("sizeof(double): %i, sizeof(uint64_t): %i\n", sizeof(double), 
            sizeof(uint64_t));

    for (i = 0; i < 4; i++) {
        u_fprintf(u_stdout, "%i times through, myStrLen: %i, uerrocode: %S\n", 
                i, myStrlen, u_uastrcpy(buffer, u_errorName(status))) ;
        myStrlen = udat_format(df, myDateArr[i], NULL, myStrlen, NULL, &status);
        if(status == U_BUFFER_OVERFLOW_ERROR){
            status = U_ZERO_ERROR;
            myString = (UChar*)malloc(sizeof(UChar) * (myStrlen+1) );
            udat_format(df, myDateArr[i], myString, myStrlen+1, NULL, &status);
            printf("double: %e - Formatted: %s\n", myDateArr[i], u_austrcpy(buffer, myString)) ;
            free(myString);
            myStrlen = 0;
        }
    }
    /* plan(3); */


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
    fclose (stdout);
    fclose (stdin) ;
}

