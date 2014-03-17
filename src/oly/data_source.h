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

#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif

#ifndef SRC_DATA_SOURCE_H
#define SRC_DATA_SOURCE_H 1

BEGIN_C_DECLS

struct data_source_struct;
typedef struct data_source_struct OlyDataSource;

typedef enum oly_ds_direction {
  IN = 0, 
  OUT = 1, 
  INOUT = 2
} OlyDSDirection;

typedef enum data_source_type_struct {
    YAML_FILE = 0,
    DS_TYPE_MIN = 0;
    DS_TYPE_MAX = 0;
} DataSourceType;

typedef enum data_source_format_struct {
    UNKNOWN,
    DATA_IN_FLAT_FILE,
    DATA_IN_DATABASE_FILE,
    LOCAL_DATABASE_SERVER,
    REMOTE_DATABASE_SERVER,
    REMOTE_DATA_CLOUD
} DataSourceFormat;

OlyDataSource *new_data_source( DataSourceType dst, OlyStatus *status );
/* practically all of the file data sources need this. */
OlyStatus set_data_filename( OlyDataSource *ds, const char *filename );
/* normally these locale and charset functions should not be necessary,
 * since we can usually deduce this information from the data source itself. */
OlyStatus set_data_locale( OlyDataSource *ds, const char *locale );
OlyStatus set_data_charset( OlyDataSource *ds, const char *charset );
/* this function, however, will be used for many remote data sources. */
OlyStatus set_data_connection_string( OlyDataSource *ds, const char *connection_string );
OlyStatus close_data_source( OlyDataSource *ds );

END_C_DECLS

#endif /* SRC_DATA_SOURCE_H */

