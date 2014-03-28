/* bash_printing.c - pretty printing functions.  License GPL 2+{{{
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
#include "oly/olytypes.h"
#include "oly/oly_dev.h"

static void reset( void );
void
print_stdout_char_color(BashColors fg, BashColors bg, BashAttr attr, const char *text)
{
	fprintf(stdout, "%c[%d;%d;%dm%s", 0x1B, attr, fg + 30, bg + 40, text);
    reset();
}

void
print_stderr_char_color(BashColors fg, BashColors bg, BashAttr attr, const char *text)
{
	fprintf(stderr, "%c[%d;%d;%dm%s", 0x1B, attr, fg + 30, bg + 40, text);
    reset();
}


static void
reset( void )
{
	fprintf(stdout, "%c[%d;%d;%dm", 0x1B, RESET, WHITE + 30, BLACK + 40);
}

