/* get_default_locale.c - Oly function to retrieve user locale. License GPL2+ {{{
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
#include <assert.h>

#include "oly/state.h"
#include "oly/core.h"

/* these used to be public but enh, why?  Not going to use them
 * elsewhere. 
 */

static int32_t count_tokens (char *s, char *delims);
static int32_t count_nondelim_chars (char *s, char *delims);
static char **token_str_to_array(char *s, char *delims, oly_status *status); 
