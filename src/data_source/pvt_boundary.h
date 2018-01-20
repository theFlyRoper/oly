/* pvt_boundary.h -- private boundary definitions. License GPL2+ {{{
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

#ifndef DATA_SOURCE_PVT_BOUNDARY_H
#define DATA_SOURCE_PVT_BOUNDARY_H 1

#include "data_source/boundary.h"

BEGIN_C_DECLS

struct oly_boundary_struct
{
    OChar           *o_now;
    OChar           *o_end;
    OChar           *o_start;
/*    OChar           *o_flush_break; */
    char            *c_now;
    char            *c_end;
    char            *c_start;
/*    char            *c_flush_break; */
    UConverter      *converter;
};

END_C_DECLS

#endif /* DATA_SOURCE_PVT_BOUNDARY_H */



