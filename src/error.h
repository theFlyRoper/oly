/* error.h -- display formatted error diagnostics of varying severity licence GPL2+ {{{
  
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

#ifndef OLY_ERROR_H
#define OLY_ERROR_H 1

BEGIN_C_DECLS

extern UFILE *u_stderr;
extern UFILE *u_stdout;
extern UFILE *u_stdin;
extern const UChar *program_name;

extern void oly_warning      (const UChar *message);
extern void oly_error        (const UChar *message);
extern void oly_fatal        (const UChar *message);

END_C_DECLS

#endif /* !OLY_ERROR_H */

