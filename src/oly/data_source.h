/* data_source.h -- data source functions. {{{
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
 *
 * }}} */

#ifndef SRC_DATA_SOURCE_H
#define SRC_DATA_SOURCE_H 1

#include "oly/common.h"
#include "oly/olytypes.h"

BEGIN_C_DECLS

struct data_source_struct;
typedef struct data_source_struct OlyDataSource;

/* data source buffer interface */
/* gets the amount of space remaining in the input buffer.  This is so we can dump large input to files.  If we have a scalar holding war and peace, I would rather it get dumped to a file, which the OlyNode can pass around instead of five hundred buffers' worth of string :) 
extern size_t get_buffer_size(OlyDataSource *ds, OlyStatus *status);
*/
/* copies a *char string to the charbuffer. */
extern OlyStatus put_in_charbuffer(OlyDataSource *ds, const char *putstr);
/* flushes the charbuffer from the specified pointer to the end. */
extern OlyStatus flush_charbuffer( OlyDataSource *ds, 
        const char *flush_start, const char *flush_end );
extern OlyStatus put_in_ocharbuffer(OlyDataSource *ds, const OChar *putstr);
/* flushes the charbuffer from the specified pointer to the end. */
extern OlyStatus flush_ocharbuffer( OlyDataSource *ds, 
        const OChar *flush_start, const OChar *flush_end );

/* the data source interface needs to provide access to the ICU converter functions. 
 * UConververtFromUCallback is the function ICU calls after failing to convert from 
 * unicode to a given codepage, and UConverterToUCallback is the other direction. */
/*
typedef void( * UConverterToUCallback)(const void *context, UConverterToUnicodeArgs *args, const char *codeUnits, int32_t length, UConverterCallbackReason reason, UErrorCode *pErrorCode)

typedef void( * UConverterFromUCallback)(const void *context, UConverterFromUnicodeArgs *args, const UChar *codeUnits, int32_t length, UChar32 codePoint, UConverterCallbackReason reason, UErrorCode *pErrorCode)
*/

/* stub definitions for the different data source interfaces */

typedef struct oly_yaml_data_struct OlyYAMLData;

typedef union oly_data_interface_union {
    OlyYAMLData *yaml_data;
} OlyDataStream;

typedef enum oly_ds_direction_enum {
  OLY_DS_IN = 0, 
  OLY_DS_OUT = 1
} OlyDSDirection;

typedef enum oly_data_format_enum {
    UNKNOWN_DATA_SOURCE_FORMAT,
    COLLECTION_DATA_SOURCE,
    TABLE_DATA_SOURCE
} OlyDataFormat;

typedef enum oly_ds_function_type_enum {
    OLYDS_INIT_FUNCTION,
    OLYDS_OPEN_FUNCTION,
    OLYDS_DELETE_FUNCTION
} OlyDSFunctionType;

typedef enum data_source_type_enum {
    YAML_FILE = 0,
    DS_TYPE_MIN = 0,
    JSON_FILE = 1,
    SQLITE_FILE = 2,
    DS_TYPE_MAX = 2
} DataSourceType;

/* data_source_options: used to describe which options are required and which are not. 
 * locale and charset are not listed because they are valid for any data source. 
 * */
typedef enum data_source_options_enum {
    DSOPT_FILE_NAME = 0x0,
    DSOPT_CONNECTION_STRING = 0x1,
    DSOPT_MAX = 0x1
} DataSourceOptions;

typedef OlyStatus DataSourceFunction(OlyDataSource *ds) ;

extern OlyStatus check_data_option( OlyDataSource *ds, DataSourceOptions ds_opt);
extern OlyStatus set_datasource_function( OlyDataSource *ds, 
    OlyDSFunctionType ds_func_type, DataSourceFunction *ds_function );
extern OlyDataSource *new_data_source( DataSourceType dst, OlyStatus *status );
extern OlyStatus close_data_source( OlyDataSource *ds );
extern OlyDataStream *get_data_interface( OlyDataSource *ds, OlyStatus *status);
extern OlyStatus set_data_interface( OlyDataSource *ds, OlyDataStream *interface);
extern OlyStatus set_data_filename( OlyDataSource *ds, const char *filename );
extern OlyDataFormat get_data_type_format( DataSourceType ds_type );

/* Marks a data source option flag as required.  Data source initializing function should call. */
extern OlyStatus set_ds_option_required( OlyDataSource *ds, DataSourceOptions option );
/* Marks a data source option flag as unused.  Data source initializing function should call. */
extern OlyStatus set_ds_option_unused( OlyDataSource *ds, DataSourceOptions option );
/* to simplify maintenance of the data source options besides locale, charset and direction, we call set_data_option */
extern OlyStatus set_data_option( OlyDataSource *ds, 
        const DataSourceOptions option, const char *value );
extern char *get_data_option( OlyDataSource *ds, const DataSourceOptions option,
        OlyStatus *status );

/* normally these locale and charset functions should not be necessary,
 * since we can usually deduce this information from the data source itself. */
OlyStatus set_data_locale( OlyDataSource *ds, const char *locale );
OlyStatus set_data_charset( OlyDataSource *ds, const char *charset );

/* retrieve locale or charset from the data source. */
OlyStatus get_data_locale( OlyDataSource *ds, const char *locale );
OlyStatus get_data_charset( OlyDataSource *ds, const char *charset );

END_C_DECLS

#endif /* SRC_DATA_SOURCE_H */

