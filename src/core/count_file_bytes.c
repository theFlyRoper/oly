/* count_file_bytes.c - retrieves file size for malloc. {{{
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
#include "sys/types.h"
#include "oly/core.h"

OlyStatus
count_file_bytes(FILE *file, size_t *file_size, Oly *oly)
{
    fpos_t pos;
    oly->state->status = OLY_OKAY;
    fseek(file, 0, SEEK_END);
    if (fgetpos(file, &pos) != 0) 
    {
        oly->state->status = OLY_ERR_FILEIO;
    }
    *file_size = (size_t)ftello(file);
    fseek(file, 0, SEEK_SET);
    return oly->state->status;
}

