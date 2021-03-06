/* globals.h -- constants header. GPL2+ {{{
  
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

/* should only be included in the main source file for a binary,
 * as it holds the defines for globals.
 */

#ifndef SRC_OLY_GLOBALS_H
#define SRC_OLY_GLOBALS_H 1

OFILE *u_stderr;
OFILE *u_stdout;
OFILE *u_stdin;
char **environ; 
Oly *oly;

#endif /* SRC_OLY_GLOBALS_H */

