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
#include "oly/messages.h"

#ifndef SRC_OLY_CORE_H
#define SRC_OLY_CORE_H 1

#ifndef OLY_SMALL_BUFFER
#define OLY_SMALL_BUFFER 256
#endif /* OLY_SMALL_BUFFER */

BEGIN_C_DECLS

struct passwd;

/* main oly structure */
typedef struct oly_t {
    oly_resource             *messages;
    ochar                    *program_name;
    oly_status                status;
    oly_locale                locale;
    struct  oly_state_t      *state;
} Oly;

void  clean_io_open(void);
char *oget_home (struct passwd *pwd);
char *oget_user_locale (void);
oly_status get_ochar_args(ochar ***result, char **source, int32_t argc);
extern void close_oly (void);
extern int cleanenv (void);

oly_status count_file_bytes(FILE *file, size_t *file_size, Oly *oly);

int   is_big_endian (void);

oly_status get_i18n_errmsg(ochar **message, const oly_status oly_errno);
/* the char * from liberr will always be len long, counting terminator */
oly_status get_i18n_errstring(ochar **message, int32_t *message_len,
        const oly_status oly_errno);
char *get_liberror(int liberr, int *len);
oly_status init_all (char *locale);

/* OFILE IO */
extern OFILE *u_stderr;
extern OFILE *u_stdout;
extern OFILE *u_stdin;

/* holds the cleaned environment for use with getenv() */
extern char **environ; 

END_C_DECLS

#endif /* SRC_OLY_CORE_H */

