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
#include "oly/node.h"

BEGIN_C_DECLS
/* OLY_CONFIG_MAIN_UNSET must stay last. we use its value to determine the end of the config list. */
typedef enum oly_config_main_items_enum 
{
    OLY_CONFIG_MAIN_BOUNDARY_BUFFER_MAX,
    OLY_CONFIG_MAIN_STRING_BUFFER_MAX,
    OLY_CONFIG_MAIN_NODE_QUEUE_MAX,
    OLY_CONFIG_MAIN_UNSET
} OlyConfigMainItem;

/* build a config value table out of this */
typedef struct oly_config_struct  
{
    OlyNodeValue value;
    long int min; /* min is used by int, float, uint */
    OlyTagType type;
} OlyConfig;


OlyStatus new_config_item ( OlyConfig **config );
void close_config_item( OlyConfig **config );
OlyStatus load_main_config( OlyConfig **config );
OlyStatus get_main_config_int(OlyConfigMainItem item, int64_t *output );
void free_main_config(void);
OlyStatus get_config_tag_type( OlyConfigMainItem item, OlyTagType *type );
OlyStatus set_config_item( char *value, OlyConfig *config_item, OlyConfigMainItem item);

END_C_DECLS

#endif /* SRC_OLY_CONFIG_H */

