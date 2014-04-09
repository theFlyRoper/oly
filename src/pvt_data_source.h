/* pvt_data_source.h -- private data source definitions.  License GPL2+ {{{
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

#ifndef PVT_DATA_SOURCE_H
#define PVT_DATA_SOURCE_H 1

#include "oly/data_source.h"
#include "data_source/oly_yaml.h"

BEGIN_C_DECLS

/* TODO: clean this up! Lots of outdated ycchyness */
struct data_source_struct {
    OlyDSDirection        direction;
    DataSourceType        ds_type;
    OlyDataFormat         ds_format;
    OlyStatus             status;
    unsigned int          required_settings;
    unsigned int          unused_settings;
    char                 *locale;
    char                 *charset;
    char                 *filename;
    char                 *options[(DSOPT_MAX+1)];
    size_t                max_buffer_size;
    char                 *key_staging;
    size_t                key_stage_max_length;
    int64_t               sequence;
    char                 *external_scalar;
    OChar                *internal_scalar;
    OlyBoundary          *boundary;
    OlyNode              *node;
    OlyStatus           (*dispatch)(OlyDataSource *ds);
    void                 *data;
};

END_C_DECLS

#endif /* PVT_DATA_SOURCE_H */

