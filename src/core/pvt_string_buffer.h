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

struct oly_string_buffer_struct
{

    OChar *buffer;
    OChar *begin_a;
    OChar *end_a;
    OChar *begin_b;
    OChar *end_b;
    unsigned int  buffer_size;
    bool    write_to_a;
};

END_C_DECLS

#endif /* SRC_CORE_PVT_STRING_BUFFER_H */

