/* set_program_name.h -- {{{
  
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

#ifndef LIBOLY_STARTUP_SET_PROGRAM_NAME_INL_H
#define LIBOLY_STARTUP_SET_PROGRAM_NAME_INL_H 1

/** @start 2 {{{ */

const OChar *program_name = NULL;


void
set_program_name (const char *path)
{
  if ( !program_name )
    program_name = xstrdup (basename (path));
}

/** @end 2 }}} */

#endif /* LIBOLY_STARTUP_SET_PROGRAM_NAME_INL_H */


