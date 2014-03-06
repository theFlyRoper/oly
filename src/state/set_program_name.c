/* set_program_name.c - set the global constant program_name. License GPL2+ {{{
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
#include <assert.h>

#include "oly/core.h"
#include "oly/state.h"
#include "oly/resources.h"

const ochar *program_name;

oly_status 
set_program_name(oly_state *state, char *path)
{
    char *prog_holder = xstrdup(basename(&path));
    assert(program_name == NULL);
    state->status = OLY_OKAY;

    program_name = (const ochar *)cstr_to_ostr(state, prog_holder);
    return state->status;
}
