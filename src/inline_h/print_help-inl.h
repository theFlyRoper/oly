/* Print_help-inl.h -- inline funciton to show help text.  License GPL2+ {{{
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

static void
usage (void){
  /* TRANSLATORS: usage output (synopsis)*/
  u_fprintf (u_stdout,"\
Usage: %S [OPTION]... [FILE]...\n", program_name);
  
}


static void
print_help (void)
{
  usage();
  /* TRANSLATORS: --help output 2 (brief description)
     no-wrap */
  u_fprintf (u_stdout,"\
Super happy!", program_name);
}
