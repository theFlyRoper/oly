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
 * }}} */

#ifndef OLY_DATA_SOURCE_H
#define OLY_DATA_SOURCE_H 1

#include "oly/common.h"
#include "oly/olytypes.h"
#include "oly/node.h"
#include "data_source/boundary.h"

BEGIN_C_DECLS

struct data_source_struct;
typedef struct data_source_struct OlyDataSource;

/* dispatch function for filled node buffers. All data sources will have one. May be used elsewhere later. */
typedef OlyStatus (*OlyDispatch)( OlyDataSource *destination ); 

/* gets the amount of space remaining in the input buffer. */
extern size_t get_max_buffer_size(OlyDataSource *ds);
extern OlyStatus set_max_buffer_size(OlyDataSource *ds, size_t mbuff);

/* the data source interface needs to provide access to the ICU converter functions. 
 * UConververtFromUCallback is the function ICU calls after failing to convert from 
 * unicode to a given codepage, and UConverterToUCallback is the other direction. */
/*
typedef void( * UConverterToUCallback)(const void *context, UConverterToUnicodeArgs *args, const char *codeUnits, int32_t length, UConverterCallbackReason reason, UErrorCode *pErrorCode)

typedef void( * UConverterFromUCallback)(const void *context, UConverterFromUnicodeArgs *args, const UChar *codeUnits, int32_t length, UChar32 codePoint, UConverterCallbackReason reason, UErrorCode *pErrorCode)
*/

typedef enum oly_ds_direction_enum {
  OLY_DS_IN = 0, 
  OLY_DS_OUT = 1
} OlyDSDirection;

typedef enum oly_data_format_enum {
    UNKNOWN_DATA_SOURCE_FORMAT,
    COLLECTION_DATA_SOURCE,
    TABLE_DATA_SOURCE
} OlyDataFormat;

typedef enum data_source_type_enum {
    YAML_FILE = 0,
    DS_TYPE_MIN = 0,
    JSON_FILE = 1,
    SQLITE_FILE = 2,
    DS_TYPE_MAX = 2
} DataSourceType;

/* data_source_options: used to describe which options are required and which are not. 
 * locale and encoding are not listed because they are valid for any data source. */
typedef enum data_source_options_enum {
    DSOPT_FILE_NAME = 0x0,
    DSOPT_CONNECTION_STRING = 0x1,
    DSOPT_MAX = 0x1
} DataSourceOptions;

extern OlyStatus check_data_option( OlyDataSource *ds, DataSourceOptions ds_opt );
OlyStatus set_ds_dispatch_function( OlyDataSource *ds, OlyDispatch function );
OlyStatus run_ds_dispatch_function( OlyDataSource *ds );
extern OlyDataSource *new_data_source( DataSourceType dst, OlyStatus *status );
extern OlyStatus close_data_source( OlyDataSource *ds );
extern OlyStatus set_data_filename( OlyDataSource *ds, const char *filename );
extern OlyDataFormat get_data_type_format( DataSourceType ds_type );
OlyStatus get_boundary( OlyDataSource *ds, OlyBoundary **bound );
OlyStatus open_ds_boundary( OlyDataSource *ds );
void reset_ds_status( OlyDataSource *ds );
OlyStatus set_data_direction( OlyDataSource *ds, OlyDSDirection ds_io);
OlyStatus get_data_source_direction (OlyDataSource *ds, OlyDSDirection *direction);

/* Marks a data source option flag as required.  Data source initializing function should call. */
extern OlyStatus set_ds_option_required( OlyDataSource *ds, DataSourceOptions option );
/* Marks a data source option flag as unused.  Data source initializing function should call. */
extern OlyStatus set_ds_option_unused( OlyDataSource *ds, DataSourceOptions option );
/* to simplify maintenance of the data source options besides locale, encoding and direction, we call set_data_option */
extern OlyStatus set_data_option( OlyDataSource *ds, 
        const DataSourceOptions option, const char *value );
extern char *get_data_option( OlyDataSource *ds, const DataSourceOptions option,
        OlyStatus *status );
extern OlyStatus get_data_source_status( OlyDataSource *ds );
extern OlyStatus set_data_source_direction (OlyDataSource *ds, OlyDSDirection direction);

/* normally these locale and encoding functions should not be necessary,
 * since we can usually deduce this information from the data source itself. */
extern OlyStatus set_data_locale( OlyDataSource *ds, const char *locale );
extern OlyStatus set_data_encoding( OlyDataSource *ds, const char *encoding );

/* TODO: separate the internal and external functions here. */
OlyStatus stage_node_key( OlyDataSource *ds, const char *key, size_t key_len );
/* Used by external interfaces to produce or consume nodes */
extern OlyStatus enqueue_ds_node( OlyDataSource *ds, void *value, OlyTagType type);
extern OlyStatus dequeue_ds_node( OlyDataSource *ds, OlyNode **node );

/* ds_descend: descends a level */
OlyStatus ds_descend(OlyDataSource *ds);
/* ds_ascend: ascends a level and finishes a sequence or mapping. */
OlyStatus ds_ascend(OlyDataSource *ds);

/* Used by oly internal to collect and dispense nodes to and from its internal queue. */
OlyStatus collect_nodes( OlyDataSource *ds, OlyNode **node_out);
OlyStatus dispense_nodes( OlyDataSource *ds, OlyNode *node_in);

/* retrieve locale or encoding from the data source. */
char *get_data_encoding( OlyDataSource *ds );
char *get_data_locale( OlyDataSource *ds );
void *get_ds_data( OlyDataSource *ds );
OlyStatus set_ds_data( OlyDataSource *ds, void *data);

END_C_DECLS

#endif /* OLY_DATA_SOURCE_H */

