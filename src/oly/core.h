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

#ifndef SRC_OLY_WRAPPERS_H
#define SRC_OLY_WRAPPERS_H 1

#ifndef OLY_SMALL_BUFFER
#define OLY_SMALL_BUFFER 256
#endif /* OLY_SMALL_BUFFER */

BEGIN_C_DECLS

struct passwd;
/* OFILE IO */
extern OFILE *u_stderr;
extern OFILE *u_stdout;
extern OFILE *u_stdin;

/* program name and primary resource bundle */
extern const ochar *program_name;
extern UResourceBundle *OlyResources;

/* holds the cleaned environment for use with getenv() */
extern char **environ; 

oly_status check_uerror(UErrorCode *status);
void clean_io_open(void);

size_t count_tokens (char *s, char *delims);
size_t count_nondelim_chars (char *s, char *delims);
oly_status get_ochar_args(ochar ***result, char **source, int32_t argc);
/* allocates a char ** array and copies each token into it. */
char **token_str_to_array(char *s, char *delims, oly_status *status) ;

char *oget_home (struct passwd *pwd);
char *oget_user_locale (void);
int   is_big_endian (void);

extern void close_oly (void);
extern void init_io(const char *locale, const char *codepage); 

extern int cleanenv (void);


/* optlist is an array of values that apply to a given option. 
typedef struct oly_optlist{
  OlyOptList *next;
  ochar **value;
  ochar *option;
} OlyOptList;
*/

/* oly_status oly_allocopt( ochar **argv, size_t argc, OlyOptList *options );
oly_status oly_parseopt( ochar **argv, size_t argc, OlyOptList *options );
oly_status oly_nextopt( ochar **argv, size_t argc, OlyOptList *options ); */
END_C_DECLS

#endif /* SRC_OLY_WRAPPERS_H */

