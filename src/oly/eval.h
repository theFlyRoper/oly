/* eval.h -- create and dispatch command vectors {{{
  
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.
   }}} */

/** @start 1 */
#ifndef OLY_EVAL_H
#define OLY_EVAL_H 1

BEGIN_C_DECLS

typedef struct {
  int   argc;            /* number of elements in ARGV */
  ochar **argv;          /* array of pointers to elements */
  size_t  lim;           /* number of bytes allocated */
} Tokens;

extern int eval       (Oly *oly, Tokens *tokens);
extern int untokenize (Oly *oly, ochar **pcommand, Tokens *tokens);
extern int tokenize   (Oly *oly, Tokens **ptokens, ochar **pcommand);

END_C_DECLS

#endif /* !OLY_EVAL_H */

/** @end 1 */

