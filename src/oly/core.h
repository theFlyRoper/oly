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

#ifndef SRC_OLY_CORE_H
#define SRC_OLY_CORE_H 1

#ifndef OLY_SMALL_BUFFER
#define OLY_SMALL_BUFFER 256
#endif /* OLY_SMALL_BUFFER */

BEGIN_C_DECLS

/* oly_status type:
 * Defines states for Oly.
 * 
 * Negative numbers and zero are non-error states
 * or warning states. Positive numbers are error 
 * states. Starts at lowest negative number and goes 
 * up. 
 *
 * Standard return state is OLY_OKAY, which you should
 * test for after calling most functions. Use others as
 * appropriate. OLY_OKAY is equal to zero.
 */

typedef enum oly_status_t {
    OLY_WARN_ERROR_NOT_FOUND=-6,
    OLY_EOF=-5,
    OLY_EXIT=-4,
    OLY_CONTINUE=-3,
    OLY_BREAK=-2,
    OLY_INCOMPLETE=-1,
    OLY_OKAY=0,
    OLY_ERR_UNSPECIFIED=1,
    OLY_ERR_SYS=2,
    OLY_ERR_LIB=3,
    OLY_ERR_INIT=4,
    OLY_ERR_NOMEM=5,
    OLY_ERR_NOPWD=6,
    OLY_ERR_NOUSER=7,
    OLY_ERR_FILEIO=8,
    OLY_ERR_READHEX=9
} oly_status;

/* locale info structure */
typedef struct locinfo_t {
  ochar          *locale;
  ochar          *lang;
  ochar          *charset;
  ochar          *country;
  void           *other;     /* Special data like currency */
} oly_locinfo;


struct passwd;

/* a general-purpose key-value linked list. */
typedef struct oly_keystore_t {
  struct oly_keystore   *next;
  ochar                 *key;
  ochar                 *value;
} oly_keystore;

size_t count_tokens (char *s, char *delims);
size_t count_nondelim_chars (char *s, char *delims);
oly_status get_ochar_args(ochar ***result, char **source, int32_t argc);
/* allocates a char ** array and copies each token into it. */
char **token_str_to_array(char *s, char *delims, oly_status *status); 

void  clean_io_open(void);
char *oget_home (struct passwd *pwd);
char *oget_user_locale (void);
int   is_big_endian (void);

oly_status get_i18n_errmsg(ochar **message, const oly_status oly_errno);
/* the char * from liberr will always be len long, counting terminator */
char *get_liberror(int liberr, int *len);
oly_status get_i18n_errstring(ochar **message, int32_t *message_len,
        const oly_status oly_errno);

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

/* OFILE IO */
extern OFILE *u_stderr;
extern OFILE *u_stdout;
extern OFILE *u_stdin;

/* program name and primary resource bundle */
extern const ochar *program_name;
extern UResourceBundle *OlyResources;

/* holds the cleaned environment for use with getenv() */
extern char **environ; 

END_C_DECLS

#endif /* SRC_OLY_CORE_H */

