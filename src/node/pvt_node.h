/* pvt_node.h -- private node definitions. License GPL2+ {{{
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

#ifndef DATA_SOURCE_PVT_NODE_H
#define DATA_SOURCE_PVT_NODE_H 1

#include "oly/node.h"
#include "oly/string_buffer.h"

BEGIN_C_DECLS

/* TODO: long, unsigned long and double are not pointers and may align differently.
 * This union should be space efficient above all, so figure out how to ensure
 * these data types all line up, ideally without a cast.  On 64 bit linux,
 * long, unsigned long and double work nicely.  */
union oly_node_value_union {
    OChar               *string_value;
    FILE                *large_binary_file;
    OFILE               *large_text_file;
    long                 int_value;
    unsigned long        uint_value;
    double               float_value;
};

struct oly_data_source_node_struct 
{
    unsigned short       depth;
    bool                 has_key;
    bool                 collection_end;
    OlyTagType     vt;
    int64_t              tuple;
    OlyNodeValue         value;
    OlyNode             *parent_node;
    OChar               *key;
    OChar               *anchor;
};

struct oly_node_queue_struct 
{
    size_t           queue_size;
    size_t           key_max_length;
    OlyNode         *in;
    OlyNode         *out;
    OlyNode         *stack;
    OlyNode         *queue_start;
    OlyNode         *queue_end;
    OlyStringBuffer *string_buffer;
};

END_C_DECLS

#endif /* DATA_SOURCE_PVT_NODE_H */

