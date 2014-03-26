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
    OLY_CONFIG_NODE_LIST_MAX
} OlyConfigItem;

typedef struct OlyConfig_struct OlyConfig;

extern OlyConfig * load_config( OlyStatus *status );

size_t get_boundary_buffer_max(void);
size_t get_main_string_buffer_max(void);
size_t get_node_queue_max(void);
size_t get_node_list_max(void);
/* void *get_config_item(OlyConfigItem record);
*/
struct OlyConfig_struct {
    OChar *key;
    OChar *value;
    /* points to a subnode, such as an array or block. */
    OlyConfig *subnode;
};

END_C_DECLS

#endif /* SRC_OLY_CONFIG_H */

