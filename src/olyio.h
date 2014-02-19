/* io.h -- oly IO systems {{{
  
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

#ifndef OLY_IO_H
#define OLY_IO_H 1

BEGIN_C_DECLS

/* optlist is an array of values that apply to a given option. 
typedef struct oly_optlist{
  OlyOptList *next;
  OChar **value;
  OChar *option;
} OlyOptList;
*/

/* init_io is part of the loader, so we expect u_stderr, u_stdin and u_stdout to be ready */
Oly_Status oly_getOCharArgs(OChar ***result, char **source, int32_t argc);
/* Oly_Status oly_allocopt( OChar **argv, size_t argc, OlyOptList *options );
Oly_Status oly_parseopt( OChar **argv, size_t argc, OlyOptList *options );
Oly_Status oly_nextopt( OChar **argv, size_t argc, OlyOptList *options ); */

END_C_DECLS

#endif /* !OLY_IO_H */

