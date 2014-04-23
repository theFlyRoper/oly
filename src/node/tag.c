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
#include "oly/olytypes.h"
#include "oly/node.h"
#include "oly/tag.h"


#define basic_tag_list \
    TAG_FACTORY(null, OLY_TAG_SCALAR_NULL) \
    TAG_FACTORY(bool, OLY_TAG_SCALAR_BOOL) \
    TAG_FACTORY(int, OLY_TAG_SCALAR_INT) \
    TAG_FACTORY(uint, OLY_TAG_SCALAR_UINT) \
    TAG_FACTORY(float, OLY_TAG_SCALAR_FLOAT) \
    TAG_FACTORY(infinity, OLY_TAG_SCALAR_INFINITY) \
    TAG_FACTORY(nan, OLY_TAG_SCALAR_NOT_A_NUMBER) \
    TAG_FACTORY(string, OLY_TAG_SCALAR_STRING)

#define TAG_FACTORY(tag_type, tag_type_enum) \
    static OlyStatus check_tag_##tag_type(OChar *input);
basic_tag_list
#undef TAG_FACTORY

#define TAG_FACTORY(tag_type, tag_type_enum) \
    static OlyStatus import_tag_##tag_type(OlyNodeValue *input, OlyNodeValue **output);
basic_tag_list
#undef TAG_FACTORY

#define TAG_FACTORY(tag_type, tag_type_enum) \
{check_tag_##tag_type, import_tag_##tag_type, tag_type_enum},
static const OlyTag basic_tag[OLY_TAG_SCALAR_STRING+1] = {
basic_tag_list
} ;
#undef TAG_FACTORY

typedef enum oly_tag_check_enum {
    OLY_TAG_CHECK_NULL,
    OLY_TAG_CHECK_BOOL_TRUE,
    OLY_TAG_CHECK_BOOL_FALSE,
    OLY_TAG_CHECK_INT,
    OLY_TAG_CHECK_UINT_OCT,
    OLY_TAG_CHECK_UINT_HEX,
    OLY_TAG_CHECK_FLOAT,
    OLY_TAG_CHECK_INFINITY,
    OLY_TAG_CHECK_NOT_A_NUMBER,
    OLY_TAG_CHECK_STRING
} OlyTagCheck;

static URegularExpression *simple_regexp[ OLY_TAG_CHECK_STRING+1 ];

static const ResourceData *init_tag_regexp(Oly *oly);

OlyStatus init_regexp_data(Oly *oly)
{
    UErrorCode u_status = U_ZERO_ERROR;
    OlyStatus status = OLY_OKAY;
    ResourceData *tag_regexp_data = init_tag_regexp(oly);
    OChar *regexp = NULL;
    uint32_t i = 0, len = 0;
    UParseError pe;
    pe.line = 0;
    pe.offset = 0;

    for (i = 0; i <= OLY_TAG_CHECK_STRING; i++)
    {
        regexp = (OChar *)ures_getStringByIndex( regexp_data, 
                i, &len, &u_status );
        if (U_FAILURE(u_status))
        {
            fprintf(stderr, "ICU Error in OLY_TAG: %s.\n", u_errorName(u_status));
            status = OLY_ERR_ILLEGAL_TAG;
        }
        simple_regexp[i] = uregex_open(regexp, -1, 0, &pe, &u_status);
    }
    return status;
}

static ResourceData *init_tag_regexp(Oly *oly)
{
#ifdef HAVE_UNICODE_URES_H
    UErrorCode u_status = U_ZERO_ERROR;
    ResourceData *retval = (ResourceData *)ures_getByKey(
            (UResourceBundle *)get_oly_resource(oly), 
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
            HANDLE_ERROR_AND_RETURN(status);
        }
    }
    return status;
}
#undef RESET_WARN

static OlyStatus
check_tag_null(OChar *input)
{
    UErrorCode  u_status = U_ZERO_ERROR;
    OlyStatus   status = OLY_OKAY;
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
    OlyStatus status = OLY_OKAY;
    UErrorCode  u_status = U_ZERO_ERROR;
    uregex_setText( simple_regexp[OLY_TAG_CHECK_INT], input, -1, &u_status );
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag int, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }

    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_INT], -1,  &u_status ) == FALSE )
    {
        status = OLY_WARN_TAG_NOT_MATCH;
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
        printf("ICU error, check tag uint hex, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }

    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_UINT_HEX], -1,  &u_status ) == TRUE )
    {
        return status;
    }
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag uint oct, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }

    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_UINT_HEX], -1,  &u_status ) == FALSE )
    {
        status = OLY_WARN_TAG_NOT_MATCH;
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
    return status;
}

static OlyStatus
check_tag_infinity(OChar *input)
{
    OlyStatus status = OLY_OKAY;
    UErrorCode  u_status = U_ZERO_ERROR;
    uregex_setText( simple_regexp[OLY_TAG_CHECK_INFINITY], input, -1, &u_status );
    
    if (U_FAILURE(u_status)) 
    {
        printf("ICU error, check tag float, Status: %s.\n", u_errorName(u_status));
        exit(EXIT_FAILURE);
    }

    if ( uregex_matches( simple_regexp[OLY_TAG_CHECK_INFINITY], -1,  &u_status ) == FALSE )
    {
        status = OLY_WARN_TAG_NOT_MATCH;
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
        printf("ICU error, check tag float, Status: %s.\n", u_errorName(u_status));
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
import_tag_null(OChar *input, OlyNodeValue **output)
{
    OlyStatus status = OLY_OKAY;
    (*output).value = NULL;
    (*output).type = OLY_TAG_SCALAR_NULL;
    return status;
}

static OlyStatus
import_tag_bool(OChar *input, OlyNodeValue **output)
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
        (*output).value.bool_value = false;
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
        (*output).value.bool_value = true;
    }
    else
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
        (*output).type = OLY_TAG_SCALAR_BOOL;
    }

    return status;
}

static OlyStatus
import_tag_int(OChar *input, OlyNodeValue **output)
{
    OlyStatus status = OLY_OKAY;
    return status;
}

static OlyStatus
import_tag_uint(OChar *input, OlyNodeValue **output)
{
    OlyStatus status = OLY_OKAY;
    return status;
}

static OlyStatus
import_tag_float(OChar *input, OlyNodeValue **output)
{
    OlyStatus status = OLY_OKAY;
    return status;
}

static OlyStatus
import_tag_infinity(OChar *input, OlyNodeValue **output)
{
    OlyStatus status = OLY_OKAY;
    return status;
}

static OlyStatus
import_tag_nan(OChar *input, OlyNodeValue **output)
{
    OlyStatus status = OLY_OKAY;
    return status;
}

static OlyStatus
import_tag_string(OChar *input, OlyNodeValue **output)
{
    OlyStatus status = OLY_OKAY;
    return status;
}

