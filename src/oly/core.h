/* oly/core.h - Basic functions, IO and other stuff. License GPL2+ {{{
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

#ifndef SRC_OLY_CORE_H
#define SRC_OLY_CORE_H 1

#include "oly/common.h"
#include "oly/config.h"
#include "oly/state.h"
#include "oly/resources.h"
#include "oly/data_source.h"

#ifndef OLY_SMALL_BUFFER
#define OLY_SMALL_BUFFER 256
#endif /* OLY_SMALL_BUFFER */

BEGIN_C_DECLS

struct passwd;

/* constructor */
OlyStatus init_oly(const char *prog, 
        const char *datadir, const char *encoding, const char *locale);

extern OChar    *get_ltdl_errmsg( int status );
extern size_t    memory_left_now(void);
extern size_t    getMemorySize( void );

extern OlyStatus count_file_bytes(FILE *file, size_t *file_size);

extern double oly_timestamp( void ) ;
extern OChar *get_default_encoding( void );
extern OChar *get_default_locale( void );
extern const OChar *get_program_name( void );
extern const OChar *get_resource_dir( void );
extern ResourceData *get_oly_resource( Oly *oly );
extern const char *char_default_encoding( void );
extern const char *char_default_locale( void );
extern OlyStatus get_OChar_args(OChar ***result, char **source, 
        int32_t argc);
extern OlyStatus set_inbound_data_source( OlyDataSource *ds );
extern OlyStatus set_outbound_data_source( OlyDataSource *ds );
extern OlyStatus get_inbound_data_source( OlyDataSource **ds );
extern OlyStatus get_outbound_data_source( OlyDataSource **ds );
extern OlyStatus oly_run( void );
OlyStatus enqueue_node_list(OlyNode **node_list, size_t node_count);
OlyStatus dequeue_node_list( OlyNode **node_list, size_t capacity, size_t node_count );
OlyStatus open_config_item( OlyConfig *config );

OlyStatus set_status(OlyState *state, const OlyStatus status);
OlyStatus get_status(OlyState *state);
OlyStatus get_main_config(OlyConfig **c);
OlyStatus init_regexp_data(Oly *oly);

END_C_DECLS

#endif /* SRC_OLY_CORE_H */

