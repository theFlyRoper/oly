/* config.c - load and parse configuration.  License GPL 2+{{{
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

#include "oly/common.h"
#include "oly/olytypes.h"
#include <stdbool.h>
#include "pvt_config.h"
#include "node/pvt_node.h"

static bool is_config_loaded(void);

bool is_config_loaded(void)
{
    return false;
}

/*
void *get_config_item(OlyConfigItem record)
{
    size_t   size_record;
    void    *item
    switch (record)
    {
        default:
            size_record =(size_t)(BUFSIZ*4); 
    }
    item = (void *)&size_record;
    return item;
}
*/

size_t
get_boundary_buffer_max(void)
{
    size_t max_buffer;
    if (is_config_loaded() == false)
    {
        max_buffer = (size_t)(BUFSIZ);
    }
    return max_buffer;
}
size_t
get_main_string_buffer_max(void)
{
    size_t max_buffer;
    if (is_config_loaded() == false)
    {
        max_buffer = (size_t)(BUFSIZ*8);
    }
    return max_buffer;
}
size_t
get_node_queue_max(void)
{
    size_t max_buffer;
    if (is_config_loaded() == false)
    {
        max_buffer = (size_t)(BUFSIZ*sizeof(OlyNode)*2);
    }
    return max_buffer;
}
size_t
get_node_list_max(void)
{
    size_t max_buffer;
    if (is_config_loaded() == false)
    {
        max_buffer = (size_t)(BUFSIZ*sizeof(OlyNode));
    }
    return max_buffer;
}

