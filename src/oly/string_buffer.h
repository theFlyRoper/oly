/* src/oly/string_buffer.h - Oly main string buffer internals License GPL2+ {{{
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

#ifndef SRC_OLY_STRING_BUFFER_H
#define SRC_OLY_STRING_BUFFER_H 1

#include "oly/common.h"
/* oly uses a bipartite buffer.  Not the most efficient, space wise, but 
 * good with chunked data, relatively easy to implement and to understand */
BEGIN_C_DECLS

struct oly_string_buffer_struct;
typedef struct oly_string_buffer_struct OlyStringBuffer;

#ifdef DEBUG_STRING_BUFFER
/* help debug values for pointers and state changes */
void report_values(OlyStringBuffer *strbuf, const char *function);
void report_values_green(OlyStringBuffer *strbuf, const char *function);
#endif
/* create the main string buffer */
OlyStatus open_string_buffer( OlyStringBuffer **strbuf );
/* free the string buffer. */
void close_string_buffer( OlyStringBuffer *strbuf );
/* reserves length space or returns OLY_ERR_BUFFER_OVERFLOW */
OlyStatus reserve_string_buffer( OlyStringBuffer *strbuf, const size_t length );
/* copies at most *length OChars into the string buffer. */
OlyStatus enqueue_to_string_buffer( OlyStringBuffer *strbuf, const OChar *string, size_t *length_out );
/* copies at most *length OChars into the destination. *length holds the number of OChars actually provided at the end. */
OlyStatus dequeue_from_string_buffer(OlyStringBuffer *strbuf, OChar **dest, 
        const size_t size_in, size_t *length );
/* Puts amount of space available into length_out. */
OlyStatus space_available(OlyStringBuffer *strbuf, size_t *length_out);

END_C_DECLS

#endif /* SRC_OLY_STRING_BUFFER_H */

