/* oly.h -- create and maintain Oly ADT License GPL2+ {{{
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

#ifndef OLY_SRC_H
#define OLY_SRC_H 1

BEGIN_C_DECLS

/* o_stdin, o_stdout and o_stderr are defined in state.h */

struct builtintab;

typedef struct statedata {
  struct statedata *next;       /* so they can be chained */
  ochar *key;        			/* used as a key to find the right data */
  void *data;                   /* associated state data */
  void (*delete) (void *data);
} OlyState;

typedef struct oly {
  ochar *result;                 /* result string */
  size_t len;                   /* bytes used by result field */
  size_t lim;                   /* bytes allocated to result field */
  struct builtintab *builtins;  /* tables of builtin functions */
  SyntaxTable **syntax;         /* dispatch table for syntax of input */
  List *syntax_init;            /* stack of syntax state initialisers */
  List *syntax_finish;          /* stack of syntax state finalizers */
  OlyState *state;              /* state data from syntax extensions */
} Oly;

/* The groundwork for this system
 * owes a great debt to the 
 * sic simple shell library, which 
 * Gary V. Vaughan, Ben Elliston, 
 * Tom Tromey and Ian Lance Taylor
 * made to support autobook.
 *
 * read here:
 * http://www.sourceware.org/autobook/
 */

Oly *oly_new           (void);
Oly *oly_delete        (Oly *stale);

int olystate_set       (
    Oly *oly,
    const ochar *key,
    void *value,
    void (*delete) (void *value)
    );

void  *olystate_get     (Oly *oly, const ochar *key);
int   olystate_clear     (Oly *oly, const ochar *key);

Oly *oly_result_clear  (Oly *oly);
const ochar *oly_result_get (Oly *oly);
Oly *oly_result_set    (Oly *oly, const ochar *value, size_t len);
Oly *oly_result_append (Oly *oly, const ochar *value);

extern void print_help (void);
extern void print_version (void);
extern int init_all (Oly *oly, char *locale);

END_C_DECLS

#endif /* OLY_SRC_H */


