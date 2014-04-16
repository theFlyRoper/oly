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

#include ""


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

static URegularExpression simple_regexp[ OLY_TAG_SCALAR_STRING+1 ];

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

    for (i = 0; i <= OLY_TAG_SCALAR_STRING; i++)
    {
        regexp = (OChar *)ures_getStringByIndex( regexp_data, 
                i, &len, &u_status );
        if (U_FAILURE(u_status))
        {
            fprintf(stderr, "ICU Error in OLY_TAG: %s.\n",
                    u_errorName(u_status));
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

static OlyStatus
check_tag_null(OChar *input)
{
    void uregex_setText 	( 	URegularExpression *  	regexp,
		const UChar *  	text,
		int32_t  	textLength,
		UErrorCode *  	status 
	) 	
    OlyStatus status = OLY_OKAY;
    return status;
}

static OlyStatus
check_tag_bool(OChar *input)
{
    OlyStatus status = OLY_OKAY;
    return status;
}

static OlyStatus
check_tag_int(OChar *input)
{
    OlyStatus status = OLY_OKAY;
    return status;
}

static OlyStatus
check_tag_uint(OChar *input)
{
    OlyStatus status = OLY_OKAY;
    return status;
}

static OlyStatus
check_tag_float(OChar *input)
{
    OlyStatus status = OLY_OKAY;
    return status;
}

static OlyStatus
check_tag_infinity(OChar *input)
{
    OlyStatus status = OLY_OKAY;
    return status;
}

static OlyStatus
check_tag_nan(OChar *input)
{
    OlyStatus status = OLY_OKAY;
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
    return status;
}

static OlyStatus
import_tag_bool(OChar *input, OlyNodeValue **output)
{
    OlyStatus status = OLY_OKAY;
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

