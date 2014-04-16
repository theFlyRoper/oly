/* oly/config.h - configuration functions License GPL2+ {{{
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

#ifndef SRC_OLY_CONFIG_H
#define SRC_OLY_CONFIG_H 1

#include "oly/common.h"
#include "oly/state.h"
#include "oly/resources.h"

BEGIN_C_DECLS

typedef enum oly_config_items_enum {
    OLY_CONFIG_ITEM_MIN=0,
    OLY_CONFIG_BOUNDARY_BUFFER_MAX=0,
    OLY_CONFIG_MAIN_STRING_BUFFER_MAX=1,
    OLY_CONFIG_NODE_QUEUE_MAX=2,
    OLY_CONFIG_NODE_LIST_MAX=3,
    OLY_CONFIG_ITEM_MAX=3
} OlyConfigItem;

typedef enum oly_config_object_enum {
    OLY_CONFIG_OBJECT_MIN=0,
    OLY_CONFIG_MAIN=0,
    OLY_CONFIG_DATA_SOURCE=1,
    OLY_CONFIG_OBJECT_MAX=1
} OlyConfigObject;

typedef enum oly_config_value_type {
    STRING_VALUE,
    INT_VALUE,
    FLOAT_VALUE
} OlyConfigValueType;

typedef union oly_config_value_union {
    OChar               *string_value;
    long long            int_value;
    double               float_value;
} OlyConfigValue;

struct OlyConfig_struct {
    OlyConfigValue **value;
} OlyConfig;

OlyStatus load_config( OlyConfig **config );

/* OlyStatus get_config_item(OlyConfigValue *item); */

END_C_DECLS

#endif /* SRC_OLY_CONFIG_H */

