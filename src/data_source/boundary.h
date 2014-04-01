/* boundary.h -- boundary buffer abstract type definitions. License GPL2+ {{{
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

/* the OlyBoundary abstract data type contains the buffer where oly's calls to ICU's
 * character conversion system take place. */

#ifndef DATA_SOURCE_BOUNDARY_H
#define DATA_SOURCE_BOUNDARY_H 1

struct oly_boundary_struct;
typedef struct oly_boundary_struct OlyBoundary;

OlyStatus open_oly_boundary(char *charset, size_t buffer_max_size, OlyBoundary **bind);
void close_oly_boundary(OlyBoundary *bind);

extern OlyStatus flush_outbound(OlyBoundary *boundary);
extern OlyStatus flush_inbound(OlyBoundary *boundary);
extern OlyStatus clear_boundary(OlyBoundary *boundary);

OlyStatus get_ochar_out( OlyBoundary *boundary, OChar **next, size_t *next_len );
OlyStatus put_char_in( OlyBoundary *boundary, const char *s, size_t len);
OlyStatus copy_ochar_buffer( OlyBoundary *source, OChar *dest, size_t limit );
OlyStatus get_boundary_out_start( OlyBoundary *source, OChar **start );

#endif /* DATA_SOURCE_BOUNDARY_H */
