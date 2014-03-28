/* src/core/pvt_string_buffer.h - Oly main string buffer internals License GPL2+ {{{
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

#ifndef SRC_CORE_PVT_STRING_BUFFER_H
#define SRC_CORE_PVT_STRING_BUFFER_H 1

#include "oly/common.h"
#include <stdbool.h>

BEGIN_C_DECLS

typedef enum oly_string_buffer_state_enum {
    WRITE_A_READ_A,
    WRITE_B_READ_A,
    WRITE_B_READ_B,
    WRITE_A_READ_B,
    EMPTY
} OlyStringBufferState;

struct oly_string_buffer_struct
{
    OChar *buffer_start;
    OChar *read_a;
    OChar *write_a;
    OChar *read_b;
    OChar *write_b;
    OChar *buffer_end;
    OChar *reserve_start;
    OChar *reserve_end;
    OlyStringBufferState state;
    bool  keep_long_strings;
};

END_C_DECLS

#endif /* SRC_CORE_PVT_STRING_BUFFER_H */

