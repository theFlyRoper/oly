/* print_version.c -- inline function to show program version.  License GPL2+ {{{
 * Copyright (C) 2012 Oly Project
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

/* Print version and copyright information.  */

void
print_version (void)
{
  u_fprintf (u_stdout,"( %s) %s\n", PACKAGE, VERSION);
  u_fprintf (u_stdout,"\
Copyright (C) %s Nobody in particular.\n\
There is NO WARRANTY, to the extent permitted by law.\n", 
"2012");
}

