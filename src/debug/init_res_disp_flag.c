/* init_res_disp_flag.c - Debug function to print keys from an ICU table resource. {{{
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

#include <unicode/ustdio.h>
#include <unicode/uloc.h>
#include <unicode/ures.h>

#include <sys/types.h>
#include <unistd.h>

#include "oly/state.h"
#include "oly/oly_dev.h"

#define URES_TREE_DEBUG 1

void init_res_disp_flag(res_disp_flag *flag)
{
    flag->all = 1;
    flag->help = 0;
    flag->version = 0;
    flag->badopt = 0;
    flag->tables = 0;
    flag->arrays = 0;
    flag->strings = 0;
    flag->binaries = 0;
    flag->aliases = 0;
    flag->integers = 0;
    flag->vectors = 0;
}

