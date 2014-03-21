/* pvt_collection_ds.h -- private collection data source definitions. License GPL2+ {{{
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

#ifndef DATA_SOURCE_PVT_COLLECTION_DS_H
#define DATA_SOURCE_PVT_COLLECTION_DS_H 1

#include "oly/collection_ds.h"

BEGIN_C_DECLS

union oly_node_value_union {
    OChar               *string_value;
    long                 int_value;
    double               float_value;
}
typedef union oly_node_value_union OlyDSValue;

struct oly_data_source_node_struct 
{
    unsigned short       current_level;
    OlyDSNodeValueType   vt;
    OlyDSNodeValueType   parent_vt;
    OChar               *key;
    OlyDSValue          *value;
    OlyDSNode           *parent_node;
    unsigned long long   tuple;
    OlyDataSource       *ds;
    OFILE               *large_object_file;
}

END_C_DECLS

#endif /* DATA_SOURCE_PVT_COLLECTION_DS_H */

