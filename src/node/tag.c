/* tag.c - Oly tag and type functions.  License GPL 2+{{{
 * Copyright (C) 2014 Oly Project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * }}} */

#include "oly/common.h"
#include <unicode/uregex.h>
#include <unicode/unum.h>

#include "oly/olytypes.h"
#include "oly/core.h"
#include "oly/node.h"
#include "oly/resources.h"
#include "oly/tag.h"

typedef enum oly_tag_check_enum {
    OLY_TAG_CHECK_NULL,
    OLY_TAG_CHECK_BOOL_TRUE,
    OLY_TAG_CHECK_BOOL_FALSE,
    OLY_TAG_CHECK_INT,
    OLY_TAG_CHECK_UINT_OCT,
    OLY_TAG_CHECK_UINT_HEX,
    OLY_TAG_CHECK_FLOAT,
    OLY_TAG_CHECK_PLUS_INFINITY,
    OLY_TAG_CHECK_MINUS_INFINITY,
    OLY_TAG_CHECK_NOT_A_NUMBER,
    OLY_TAG_CHECK_STRING
} OlyTagCheck;

#define basic_tag_list \
    TAG_FACTORY(null, OLY_TAG_SCALAR_NULL) \
    TAG_FACTORY(bool, OLY_TAG_SCALAR_BOOL) \
    TAG_FACTORY(int, OLY_TAG_SCALAR_INT) \
    TAG_FACTORY(uint, OLY_TAG_SCALAR_UINT) \
    TAG_FACTORY(float, OLY_TAG_SCALAR_FLOAT) \
    TAG_FACTORY(plus_inf, OLY_TAG_SCALAR_PLUS_INFINITY) \
    TAG_FACTORY(minus_inf, OLY_TAG_SCALAR_MINUS_INFINITY) \
    TAG_FACTORY(nan, OLY_TAG_SCALAR_NOT_A_NUMBER) \
    TAG_FACTORY(string, OLY_TAG_SCALAR_STRING)

#define TAG_FACTORY(tag_type, tag_type_enum) \
static OlyStatus check_tag_##tag_type(OChar *input);
basic_tag_list
#undef TAG_FACTORY

#define TAG_FACTORY(tag_type, tag_type_enum) \
static OlyStatus import_tag_##tag_type(OChar *input, OlyNodeValue *output);
basic_tag_list
#undef TAG_FACTORY

#define TAG_FACTORY(tag_type, tag_type_enum) \
{check_tag_##tag_type, import_tag_##tag_type, tag_type_enum},
static const OlyTag basic_tag[OLY_TAG_SCALAR_STRING+1] = {
basic_tag_list
} ;
#undef TAG_FACTORY
/* Create a formatter for the selected locale */
static URegularExpression *simple_regexp[ OLY_TAG_CHECK_STRING ];
static UNumberFormat *num_import;
static ResourceData *init_tag_regexp(Oly *oly);

OlyStatus init_regexp_data(Oly *oly_data)
{
    UErrorCode u_status = U_ZERO_ERROR;
    OlyStatus status = OLY_OKAY;
    ResourceData *tag_regexp_data = init_tag_regexp(oly_data);
    OChar *regexp = NULL;
    int32_t i = 0, len = 0;
    const char *locale = char_default_locale( );
    UParseError pe;
    pe.line = 0;
    pe.offset = 0;
    
    num_import = unum_open( UNUM_DECIMAL, 0, 0, (char *)locale, &pe, &u_status );
    if (U_FAILURE(u_status)) {
        fprintf(stderr, "unum_open, Errname: %s, line %d, column %d\n", 
                u_errorName(status), (int)pe.line, (int)pe.offset);
        exit(EXIT_FAILURE);
    }


    for (i = 0; i < OLY_TAG_CHECK_STRING; i++)
    {
        regexp = (OChar *)ures_getStringByIndex( tag_regexp_data, i, &len, &u_status );
        if (U_FAILURE(u_status))
        {
            fprintf(stderr, "ICU Error in OLY_TAG: %s.\n", u_errorName(u_status));
            status = OLY_ERR_ILLEGAL_TAG;
        }
        simple_regexp[i] = uregex_open(regexp, -1, 0, &pe, &u_status);
        if (U_FAILURE(u_status))
        {
            fprintf(stderr, "ICU Error in OLY_TAG: %s.\n", u_errorName(u_status));
            fprintf(stderr, "at row: %i, column: %i.\n", pe.line, pe.offset);
            status = OLY_ERR_ILLEGAL_TAG;
        }
    }
    return status;
}

static ResourceData *init_tag_regexp(Oly *oly_data)
{
#ifdef HAVE_UNICODE_URES_H
    UErrorCode u_status = U_ZERO_ERROR;
    ResourceData *retval = (ResourceData *)ures_getByKey(
            (UResourceBundle *)get_oly_resource(oly_data), 
            "SimpleTagRegExp", NULL, &u_status);
    if (U_FAILURE(u_status))
    {
        printf("Error tag_regexp not found! Status: %s.\n",
                u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    return retval;
#endif /* HAVE_UNICODE_URES_H */
}

OlyStatus infer_simple_tag( OChar *scalar, OlyTagType *type_out )
{
    OlyStatus status = OLY_OKAY;
    int i = 0;
    for ( i = 0; i <= OLY_TAG_SCALAR_STRING; i++ )
    {
        status = basic_tag[i].check(scalar);
        if (status == OLY_OKAY)
        {
            *type_out = (OlyTagType)i;
            break;
        } 
        else if (status == OLY_WARN_TAG_NOT_MATCH)
        {
            status = OLY_OKAY;
        }
        else
        {
            HANDLE_STATUS_AND_RETURN(status);
        }
    }
    return status;
}

OlyStatus convert_tag( OChar *scalar, OlyTagType *tag_type, OlyNodeValue *value)
{
    OlyStatus status = OLY_OKAY;
    
    if (tag_type == NULL)
    {
        status = infer_simple_tag(scalar, tag_type);
    }
    if (status == OLY_OKAY)
    {
        status = basic_tag[*tag_type].import(scalar, value);
    }
    else
    {
        HANDLE_STATUS_AND_RETURN( status );
    }
    return status;
}

static OlyStatus
check_tag_null(OChar *input)
{
    UErrorCode  u_status = U_ZERO_ERROR;
    OlyStatus   status = OLY_OKAY;
    /* check if the input is empty (a null value) */
    if (*input == (OChar)0 )
    {
        return status;
    }

    uregex_setText( simple_regexp[OLY_TAG_CHECK_NULL], input, -1, &u_status );
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag null, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }

    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_NULL], -1,  &u_status ) == FALSE )
    {
        status = OLY_WARN_TAG_NOT_MATCH;
    }
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag null after check, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    
    return status;
}

static OlyStatus
check_tag_bool(OChar *input)
{
    UErrorCode  u_status = U_ZERO_ERROR;
    OlyStatus   status = OLY_OKAY;
    uregex_setText( simple_regexp[OLY_TAG_CHECK_BOOL_FALSE], input, -1, &u_status );
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag bool, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_BOOL_FALSE], -1,  &u_status ) == TRUE )
    {
        return status;
    }
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag bool, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }

    uregex_setText( simple_regexp[OLY_TAG_CHECK_BOOL_TRUE], input, -1, &u_status );
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag bool, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_BOOL_TRUE], -1,  &u_status ) == FALSE )
    {
        status = OLY_WARN_TAG_NOT_MATCH;
    }
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag bool, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }

    return status;
}

static OlyStatus
check_tag_int(OChar *input)
{
    OlyStatus status = OLY_WARN_TAG_NOT_MATCH;
    UErrorCode  u_status = U_ZERO_ERROR;
    uregex_setText( simple_regexp[OLY_TAG_CHECK_INT], input, -1, &u_status );
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag int, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }

    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_INT], -1,  &u_status ) == TRUE )
    {
        status = OLY_OKAY;
    }

    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag int after match, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    return status;
}

static OlyStatus
check_tag_uint(OChar *input)
{
    OlyStatus status = OLY_OKAY;
    UErrorCode  u_status = U_ZERO_ERROR;
    uregex_setText( simple_regexp[OLY_TAG_CHECK_UINT_OCT], input, -1, &u_status );
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag uint oct, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }

    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_UINT_OCT], -1,  &u_status ) == TRUE )
    {
        return status;
    }
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag uint oct, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    
    uregex_setText( simple_regexp[OLY_TAG_CHECK_UINT_HEX], input, -1, &u_status );
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag uint hex, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }

    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_UINT_HEX], -1,  &u_status ) == FALSE )
    {
        status = OLY_WARN_TAG_NOT_MATCH;
    }
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag uint hex, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    return status;
}

static OlyStatus
check_tag_float(OChar *input)
{
    OlyStatus status = OLY_OKAY;
    UErrorCode  u_status = U_ZERO_ERROR;
    uregex_setText( simple_regexp[OLY_TAG_CHECK_FLOAT], input, -1, &u_status );
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag float, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }

    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_FLOAT], -1,  &u_status ) == FALSE )
    {
        status = OLY_WARN_TAG_NOT_MATCH;
    }
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag float after check: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    return status;
}

static OlyStatus
check_tag_plus_inf(OChar *input)
{
    OlyStatus status = OLY_OKAY;
    UErrorCode  u_status = U_ZERO_ERROR;
    uregex_setText( simple_regexp[OLY_TAG_CHECK_PLUS_INFINITY], input, -1, &u_status );
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check plus inf, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }

    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_PLUS_INFINITY], -1,  &u_status ) == FALSE )
    {
        status = OLY_WARN_TAG_NOT_MATCH;
    }
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check plus inf, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    return status;
}

static OlyStatus
check_tag_minus_inf(OChar *input)
{
    OlyStatus status = OLY_OKAY;
    UErrorCode  u_status = U_ZERO_ERROR;
    uregex_setText( simple_regexp[OLY_TAG_CHECK_MINUS_INFINITY], input, -1, &u_status );
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check minus inf, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }

    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_MINUS_INFINITY], -1,  &u_status ) == FALSE )
    {
        status = OLY_WARN_TAG_NOT_MATCH;
    }
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check minus inf, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    return status;
}

static OlyStatus
check_tag_nan(OChar *input)
{
    OlyStatus status = OLY_OKAY;
    UErrorCode  u_status = U_ZERO_ERROR;
    uregex_setText( simple_regexp[OLY_TAG_CHECK_NOT_A_NUMBER], input, -1, &u_status );
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag nan, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }

    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_NOT_A_NUMBER], -1,  &u_status ) == FALSE )
    {
        status = OLY_WARN_TAG_NOT_MATCH;
    }
    return status;
}

static OlyStatus
check_tag_string(OChar *input)
{
    OlyStatus status = OLY_OKAY;
    return status;
}

static OlyStatus
import_tag_null(OChar *input, OlyNodeValue *output)
{
    OlyStatus status = OLY_OKAY;
    output->value.string_value = NULL;
    output->type = OLY_TAG_SCALAR_NULL;
    return status;
}

static OlyStatus
import_tag_bool(OChar *input, OlyNodeValue *output)
{
    UErrorCode  u_status = U_ZERO_ERROR;
    OlyStatus   status = OLY_OKAY;
    uregex_setText( simple_regexp[OLY_TAG_CHECK_BOOL_FALSE], input, -1, &u_status );
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag bool, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_BOOL_FALSE], -1,  &u_status ) == TRUE )
    {
        output->value.bool_value = false;
    }
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag bool, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }

    uregex_setText( simple_regexp[OLY_TAG_CHECK_BOOL_TRUE], input, -1, &u_status );
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag bool, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_BOOL_TRUE], -1,  &u_status ) == TRUE )
    {
        output->value.bool_value = true;
    }
    else if ( output->value.bool_value != false )
    {
        status = OLY_WARN_TAG_NOT_MATCH;
    }
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag bool, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    if (status == OLY_OKAY)
    {
        output->type = OLY_TAG_SCALAR_BOOL;
    }

    return status;
}

static OlyStatus
import_tag_int(OChar *input, OlyNodeValue *output)
{
    OlyStatus status = OLY_OKAY;
    int32_t pos = 0;
    UErrorCode  u_status = U_ZERO_ERROR;
    output->value.int_value = (long)unum_parse(num_import, input, -1, &pos, &u_status);
    if (U_FAILURE(u_status))
    {
        printf("ICU error, import tag int, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    
    if (input[pos] != (OChar)0)
    {
        output->value.string_value = NULL;
        output->type = OLY_TAG_TYPE_UNSET;
        status = OLY_WARN_TAG_NOT_MATCH;
    }
    else
    {
        output->type = OLY_TAG_SCALAR_INT;
    }

    return status;
}

static OlyStatus import_tag_uint(OChar *input, OlyNodeValue *output)
{
    OlyStatus status = OLY_OKAY;
    long intlong = 0;
    if ((*(input + 1) == 'x') || (*(input + 1) == 'X'))
    {
        u_sscanf(input, "%x", &intlong);
    }
    /* code for octal.  Didn't work right. */
    /*
    else if ((*(input + 1) == 'o') || (*(input + 1) == 'O'))
    {
        u_fprintf(u_stderr, "converting ");
        u_fprintf_u(u_stderr, input);
        u_sscanf(input, "%o", &intlong);
        u_fprintf(u_stderr, " to %u\n",intlong);
    }
    */
    else
    {
        status = OLY_WARN_TAG_NOT_MATCH;
    }
    if (status == OLY_OKAY)
    {
        output->value.uint_value = (unsigned long)intlong;
        output->type = OLY_TAG_SCALAR_UINT;
    }
    else 
    {
        output->value.string_value = NULL;
        output->type = OLY_TAG_TYPE_UNSET;
    }

    return status;
}

static OlyStatus
import_tag_float(OChar *input, OlyNodeValue *output)
{
    OlyStatus status = OLY_OKAY;
    int32_t pos = 0;
    UErrorCode  u_status = U_ZERO_ERROR;
    output->value.float_value = unum_parseDouble(num_import, input, -1, &pos, &u_status);
    if (U_FAILURE(u_status))
    {
        printf("ICU error, import tag float, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    
    if (input[pos] != (OChar)0)
    {
        output->value.string_value = NULL;
        output->type = OLY_TAG_TYPE_UNSET;
        status = OLY_WARN_TAG_NOT_MATCH;
    }
    else
    {
        output->type = OLY_TAG_SCALAR_FLOAT;
    }

    return status;
}

static OlyStatus
import_tag_plus_inf(OChar *input, OlyNodeValue *output)
{
    UErrorCode  u_status = U_ZERO_ERROR;
    OlyStatus   status = OLY_OKAY;
    uregex_setText( simple_regexp[OLY_TAG_CHECK_PLUS_INFINITY], input, -1, &u_status );
    if (U_FAILURE(u_status))
    {
        printf("ICU error, check tag infinity, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_PLUS_INFINITY], -1,  &u_status ) == TRUE )
    {
        output->value.string_value = NULL;
        output->type = OLY_TAG_SCALAR_PLUS_INFINITY;
    }
    else 
    {
        output->type = OLY_TAG_TYPE_UNSET;
        status = OLY_WARN_TAG_NOT_MATCH;
    }
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag infinity, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    return status;
}

static OlyStatus
import_tag_minus_inf(OChar *input, OlyNodeValue *output)
{
    UErrorCode  u_status = U_ZERO_ERROR;
    OlyStatus   status = OLY_OKAY;
    uregex_setText( simple_regexp[OLY_TAG_CHECK_MINUS_INFINITY], input, -1, &u_status );
    if (U_FAILURE(u_status))
    {
        printf("ICU error, check tag infinity, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_MINUS_INFINITY], -1,  &u_status ) == TRUE )
    {
        output->value.string_value = NULL;
        output->type = OLY_TAG_SCALAR_MINUS_INFINITY;
    }
    else 
    {
        output->type = OLY_TAG_TYPE_UNSET;
        status = OLY_WARN_TAG_NOT_MATCH;
    }
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag infinity, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    return status;
}

static OlyStatus
import_tag_nan(OChar *input, OlyNodeValue *output)
{
    UErrorCode  u_status = U_ZERO_ERROR;
    OlyStatus   status = OLY_OKAY;
    uregex_setText( simple_regexp[OLY_TAG_CHECK_NOT_A_NUMBER], input, -1, &u_status );
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, import tag NaN, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_NOT_A_NUMBER], -1,  &u_status ) == TRUE )
    {
        output->value.string_value = NULL;
        output->type = OLY_TAG_SCALAR_NOT_A_NUMBER;
    }
    else 
    {
        output->type = OLY_TAG_TYPE_UNSET;
        status = OLY_WARN_TAG_NOT_MATCH;
    }
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, import tag NaN, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
    return status;
}

/* sets the value.string_value pointer to the value of input.  Does not copy the string. */
static OlyStatus
import_tag_string(OChar *input, OlyNodeValue *output)
{
    OlyStatus status = OLY_OKAY;
    output->value.string_value = input;
    output->type = OLY_TAG_SCALAR_STRING;
    return status;
}

