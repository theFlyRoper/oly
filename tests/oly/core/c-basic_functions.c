/* c-basic_functions - tests for functions in src/core/basic_functions.c {{{
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
#include <unicode/udat.h>
#include <yaml.h>
#include "oly/core.h"
#include "oly/globals.h"
#include "pvt_core.h"

#include "tests/tap/basic.h"
#include "tests/tap/float.h"
/* MAIN */
int
main( int argc, char **argv )
{
    size_t  results[] = {4255, 142803, 46838, 0}, i = 0,
            output = 0;
    const size_t b = 5;
    const char *count_bytes_files[] = {
            "/data/orig_breaks.txt", "/data/spawner_output.txt",
            "/data/zh_romance_of_three_kingdoms.txt", ""},
            *source_dir = getenv("SOURCE");
    char    buffer[BUFSIZ], tinybuffer[b], *locale = "root", *charset = "UTF-8";
    OChar   obuffer[BUFSIZ], otinybuffer[b];
    struct  stat sts;
    time_t  now;
    FILE    *f;
    init_oly(argv[0], TEST_PKGDATADIR, charset, locale, &oly);
    if (source_dir == NULL)
    {
        fprintf(stderr, "requires SOURCE environment variable, supplied by runtest. Exiting...\n");
        exit(EXIT_FAILURE);
    }
    plan(16);
    diag("----- Testing oly_timestamp function. -----");
    time(&now);
    is_double(((double)( now*1000)), oly_timestamp(), 1000.0, "Two times should be close enough.");

    diag("----- Testing count_file_bytes function. -----");
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
    
    diag("----- Testing get_default_locale and get_default_charset function. -----");
    is_unicode_string( u"root", 
            get_default_locale(), "For this test, default locale should be root.");
    is_unicode_string( u"UTF-8", 
            get_default_charset(), "For this test, default charset should be UTF-8.");
    
    diag("----- Testing char_default_locale and char_default_charset function. -----");
    is_string( "root", 
            char_default_locale(), "For this test, default locale should be root.");
    is_string( "UTF-8", 
            char_default_charset(), "For this test, default charset should be UTF-8.");
    
    diag("----- ostr_to_cstr and cstr_to_ostr -----");
    is_unicode_string( u"Lorum ipsum etc...", 
            cstr_to_ostr( obuffer, BUFSIZ, "Lorum ipsum etc..."), 
            "Char to ochar with space, BUFSIZ = %i", BUFSIZ);
    is_unicode_string( u"Loru", 
            cstr_to_ostr( otinybuffer, b, "Lorum ipsum etc..." ), 
            "Char to ochar with tiny buffer of %i characters.", (int)b );
    is_unicode_string( u"", 
            cstr_to_ostr( otinybuffer, b, "" ), 
            "Char to ochar With empty string" );
    is_unicode_string( u"", 
            cstr_to_ostr( otinybuffer, b, NULL ), 
            "Char to ochar from null" );
    is_string( "Back the other way!", 
            ostr_to_cstr( buffer, BUFSIZ, u"Back the other way!" ), 
            "OChar to char, BUFSIZ = %i", BUFSIZ );
    is_string( "five", 
            ostr_to_cstr( tinybuffer, b, u"five characters is all that fits." ), 
            "OChar to char, buffer of %i OChars", (int)b );
    is_string( "", 
            ostr_to_cstr( tinybuffer, b, NULL ), 
            "Char to ochar a NULL from string." );
    is_string( "", 
            ostr_to_cstr( tinybuffer, b, u"" ), 
            "Char to ochar with an empty string." );


    exit(EXIT_SUCCESS);
}

