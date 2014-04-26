/* c-infer_simple_tag.c - tests for infer_simple_tag function.  License GPL2+ {{{
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
#include "oly/common.h"

#include <stdbool.h>
#include "oly/olytypes.h"
#include "oly/core.h"
#include "oly/globals.h"
#include "pvt_core.h"

static OlyStatus check_bool_result(OlyNodeValue v, int extern_test_num);

int
main( int argc, char **argv )
{
    OlyStatus        status  = OLY_OKAY;
    OlyTagType       tag_type = OLY_TAG_TYPE_UNSET;
    char            *locale = NULL, *encoding = NULL, *locdir = (char *)PKGDATADIR; 
    OlyNodeValue     check_node;
    const char      *test_tags[] = { 
                        /* correct null */
                        "NULL", "Null", "null", "~", 
                        /* a string that should be null */
                        "",
                        /* null with bad caps, string. */
                        "NuLl", 
                        /* correct booleans */
                        "True", "TRUE", "true", "False", "FALSE", "false", 
                        /* fake booleans */
                        "FaLsE", "TrUe", "TRue", "FALse", 
                        /* correct decimal integers */
                        "172398", "444", "-173", "-12974889123576", "+2",
                        /* plus or minus alone. */
                        "-", "+",
                        /* correct unsigned integers, hex */
                        "0x555",  "0xabbaABBA58", 
                        /* correct unsigned integers, octal */
                        "0o271476", "0o0012",
                        /* bad octal with 9s in it. */
                        "0o2914906", 
                        /* correct floats */
                        "1.1165356", "-3.141592", "99e25", "9.297E33", "2.2812e-21",
                        /* bad floats */
                        "e22", "3187ee33", 
                        /* correct inifinity */
                        ".inf", ".INF", ".Inf",
                        /* bad inifinity */
                        ".InF", ".iNf",
                        /* correct not-a-number */
                        ".nan", ".NAN", ".NaN",
                        /* bad not-a-number */
                        ".Nan", ".naan", ".nAN",
                        /* well, thanks to the above, we already know that strings work well,
                         * here are a few mah-jong tiles which are high codepoint characters. */
                        "🀦", "🀀", "🀄",
                        NULL };
    const int        tag_result[] = {
                        /* correct null */
                        0, 0, 0, 0, 
                        /* a string that should be a null */
                        0,
                        /* null with bad caps, string. */
                        7,
                        /* correct booleans */
                        1, 1, 1, 1, 1, 1, 
                        /* fake booleans */
                        7, 7, 7, 7, 
                        /* correct decimal integers */
                        2, 2, 2, 2, 2,
                        /* plus or minus alone. */
                        7, 7,
                        /* correct unsigned integers, hex */
                        3, 3,
                        /* correct unsigned integers, octal */
                        3, 3,
                        /* bad octal with 9s in it. */
                        7,
                        /* correct floats */
                        4, 4, 4, 4, 4,
                        /* fake floats */
                        7, 7,
                        /* correct inifinity */
                        5, 5, 5,
                        /* bad inifinity */
                        7, 7,
                        /* correct not-a-number */
                        6, 6, 6,
                        /* bad not-a-number */
                        7, 7, 7,
                        /* High codepoint characters */
                        7, 7, 7 };
    const char      *tag_type_list[] = { "OLY_TAG_SCALAR_NULL", "OLY_TAG_SCALAR_BOOL",
                        "OLY_TAG_SCALAR_INT", "OLY_TAG_SCALAR_UINT", "OLY_TAG_SCALAR_FLOAT",
                        "OLY_TAG_SCALAR_INFINITY", "OLY_TAG_SCALAR_NOT_A_NUMBER",
                        "OLY_TAG_SCALAR_STRING" };
    OChar            test_buffer[BUFSIZ];
    int              i = 0;
    UErrorCode u_status = U_ZERO_ERROR;
    check_node.value.string_value = NULL;
    check_node.type = OLY_TAG_TYPE_UNSET;
    
    for ( i = 0; (test_tags[i] != NULL); i++ )
        ;
    plan(i*3);
    status = init_oly(argv[0], locdir, encoding, locale);
    for ( i = 0; (test_tags[i] != NULL); i++ )
    {
        u_strFromUTF8( test_buffer, BUFSIZ, NULL, test_tags[i], -1, &u_status );
        if (U_FAILURE(u_status))
        {
            printf("Error test_buffer %s.\n",
                    u_errorName(u_status));
            exit(EXIT_FAILURE);
        }
        status = infer_simple_tag(test_buffer, &tag_type);
        is_int(tag_result[i], tag_type, "%s", tag_type_list[tag_result[i]]);
        HANDLE_STATUS_AND_DIE(status);
        
        status = convert_tag(test_buffer,  tag_type);
        HANDLE_STATUS_AND_DIE(status);
        switch (tag_type)
        {
            case OLY_TAG_SCALAR_NULL:
                ok(NULL == check_node.value.string_value, "test %i, check null", i);
                ok(OLY_TAG_SCALAR_NULL == check_node.type, "test %i, check null", i);
                break;
            case OLY_TAG_SCALAR_BOOL:
                status = check_bool_result(check_node, i);
                HANDLE_STATUS_AND_DIE(status);                

            case OLY_TAG_SCALAR_INT:
            case OLY_TAG_SCALAR_UINT:
            case OLY_TAG_SCALAR_FLOAT:
            case OLY_TAG_SCALAR_INFINITY:
                ok(NULL == check_node.value.string_value, "test %i, check infinity", i);
                ok(OLY_TAG_SCALAR_INFINITY == check_node.type, "test %i, check infinity", i);
            case OLY_TAG_SCALAR_NOT_A_NUMBER:
                ok(NULL == check_node.value.string_value, "test %i, check not_a_number", i);
                ok(OLY_TAG_SCALAR_NOT_A_NUMBER == check_node.type, "test %i, check not_a_number", i);
            case OLY_TAG_SCALAR_STRING:
                break;
            default:
                break;
        }
    }

    HANDLE_STATUS_AND_DIE(status);
    return EXIT_SUCCESS;
}

static OlyStatus check_bool_result(OlyNodeValue v, int extern_test_num)
{
    const bool bool_results[] = { true, true, true, false, false, false };
    static int test = 0;
    OlyStatus status = OLY_OKAY;
    if (test > 5)
    {
        status = OLY_ERR_BUFFER_OVERFLOW;
        return status;
    }

    ok(v.value.bool_value == bool_results[test], "bool test %i, check bool", extern_test_num);
    ok(OLY_TAG_SCALAR_NULL == v.type, "test %i, check bool", extern_test_num);
    test++;
    return status;
}


