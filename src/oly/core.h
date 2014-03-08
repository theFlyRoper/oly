/* core.h - Basic functions, IO and other stuff. License GPL2+ {{{
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
#include "oly/state.h"
#include "oly/resources.h"

#ifndef SRC_OLY_CORE_H
#define SRC_OLY_CORE_H 1

#ifndef OLY_SMALL_BUFFER
#define OLY_SMALL_BUFFER 256
#endif /* OLY_SMALL_BUFFER */

BEGIN_C_DECLS

struct passwd;

#ifndef OLY_TOP_RESOURCE
#define OLY_TOP_RESOURCE "primary"
#endif /* OLY_TOP_RESOURCE */
/* main oly structure */
struct oly_struct; 
typedef struct oly_struct Oly;

size_t memory_left_now(void);
size_t getMemorySize( );
Oly     *new_oly(void);
extern   oly_status count_file_bytes(FILE *file, size_t *file_size, Oly *oly);

extern int   is_big_endian (void);
extern oly_status init_oly(Oly *oly, char *prog, char *datadir, char *charset, char *locale);

extern char *get_oly_charset(Oly *oly);
extern char *get_oly_locale(Oly *oly);
extern oly_status get_ochar_args(ochar ***result, char **source, int32_t argc);

/* OFILE IO */
extern OFILE *u_stderr;
extern OFILE *u_stdout;
extern OFILE *u_stdin;
extern const ochar *program_name; 

/* holds the cleaned environment for use with getenv() */
extern char **environ; 

END_C_DECLS

#endif /* SRC_OLY_CORE_H */

