/* pvt_hash.h -- private hash definitions. License GPL2+ {{{
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

#ifndef DATA_SOURCE_PVT_HASH_H
#define DATA_SOURCE_PVT_HASH_H 1

#include "oly/hash.h"
#include "oly/olytypes.h"

BEGIN_C_DECLS

typedef union oly_hash_ptr_union
{
    OlyHashTableColumn *table;
    OlyTag *tag;
    OlyAnchor *anchor;
    OlyKey *key;
} OlyHashPtr;

/* assuming an 8 bit char, each hash table will have 128 potential entries (that is, 2^7 + stop bit).  stop bit means we have most significant bit = 0, so table slots from 128-255 (starting from zero) indicate we must use next hash chunk.  256 pointers = 2048 bytes on 64 bit system.
 * For simplicity, we will use 7 of 8 bits in each byte in a char for the hash.  We lose 32 bits this way, but enh.
 * The stop bit tells whether the pointer is pointing to the end goal object or to the next layer of hash table.  Zero means it is pointing to the next layer, one means it is pointing to an object.
 * Idea for stop bits comes from Google protocol buffer varints: https://developers.google.com/protocol-buffers/docs/encoding#varints */

struct oly_hash_table_struct
{
    OlyHashPtr          *start;
    OlyHashPtr          *end;
    OlyHashPtr          *next_open;
} OlyHashTable;

/* space estimates:
 * One hash requires 28 bytes to store.  We won't be storing them for now.
 * L1: 1 * 2048
 *      Holds 128 objects
 *      requires 3584 bytes to store all hashes in memory.
 * L2: (128 + 1) * 2048 = 264,192
 *      Holds 16,384 objects
 *      requires 458,752 bytes to store all hashes in memory.
 * L3: (128^2 + 128 + 1) * 2048 = 33,818,624
 *      Holds 2,097,152 objects
 *      requires 58,720,256 bytes to store all hashes in memory.
 * L4: (128^3 + 128^2 + 128 + 1) * 2048 = 4,328,785,920
 *      Holds 268,435,456 objects.
 *      Requires 7,516,192,768 bytes to store all hashes in memory.
 */

END_C_DECLS

#endif /* DATA_SOURCE_PVT_HASH_H */


