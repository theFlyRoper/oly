/* pvt_tag.h -- private tag definitions. License GPL2+ {{{
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

#ifndef DATA_SOURCE_PVT_tag_H
#define DATA_SOURCE_PVT_tag_H 1

#include "oly/tag.h"
#include "oly/string_buffer.h"

BEGIN_C_DECLS

/* TODO: long, unsigned long and double are not pointers and may align differently.
 * This union should be space efficient above all, so figure out how to ensure
 * these data types all line up, ideally without a cast.  On 64 bit linux,
 * long, unsigned long and double work nicely.  */

struct oly_node_value_struct
{
    union oly_node_value_union 
    {
        OlyNode             **collection_value;
        OChar                *string_value;
        FILE                 *large_binary_file;
        OFILE                *large_text_file;
        double                float_value;
        long                  int_value;
        unsigned long         uint_value;
    } value;
    OlyTagType   type;
};

END_C_DECLS

#endif /* DATA_SOURCE_PVT_tag_H */


