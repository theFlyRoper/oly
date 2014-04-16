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
    OLY_CONFIG_BOUNDARY_BUFFER_MAX,
    OLY_CONFIG_MAIN_STRING_BUFFER_MAX,
    OLY_CONFIG_NODE_QUEUE_MAX,
    OLY_CONFIG_NODE_LIST_MAX,
    OLY_CONFIG_ITEM_MAX
} OlyConfigItem;

typedef enum oly_config_value_type {
    STRING_VALUE,
    INT_VALUE,
    FLOAT_VALUE
} OlyConfigValueType;

typedef struct oly_config_value_struct {
    union oly_config_value_union {
        OChar               *string_value;
        long long            int_value;
        double               float_value;
    };
    OlyConfigValueType type;
} OlyConfigValue;

struct OlyConfig_struct {
    OlyConfigValue **value;
} OlyConfig;

OlyStatus load_config( OlyConfig **config );
OlyStatus find_config_item( OChar *source, OlyConfigItem *item);
OlyStatus set_config_value( OlyConfig **config, OlyConfigItem item, 
    OlyConfigValue *value );

/* OlyStatus get_config_item(OlyConfigValue *item); */

END_C_DECLS

#endif /* SRC_OLY_CONFIG_H */

