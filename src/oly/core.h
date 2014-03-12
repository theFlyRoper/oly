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
#include "oly/load_config.h"
#include "oly/state.h"
#include "oly/resources.h"

#ifndef OLY_SMALL_BUFFER
#define OLY_SMALL_BUFFER 256
#endif /* OLY_SMALL_BUFFER */

BEGIN_C_DECLS

struct passwd;

/* constructor */
extern Oly *init_oly(const char *prog, 
        const char *datadir, const char *charset, const char *locale);

OChar *get_errmsg( OlyStatus status );
size_t   memory_left_now(void);
size_t   getMemorySize( void );


extern int          is_big_endian (void);
extern OlyStatus    count_file_bytes(FILE *file, size_t *file_size, 
                        Oly *oly);

extern double oly_timestamp( void ) ;
extern OChar *get_default_charset(Oly *oly);
extern OChar *get_default_locale(Oly *oly);
extern OlyStatus get_OChar_args(OChar ***result, char **source, 
        int32_t argc);

END_C_DECLS

#endif /* SRC_OLY_CORE_H */

