/* pvt_core.h -- Internals for oly.  GPL2+ {{{
  
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

#include "oly/core.h"
/* internal API */
#ifndef SRC_PVT_OLY_H
#define SRC_PVT_OLY_H 1
BEGIN_C_DECLS
/* I tinkered with one-function-per-file for a while, perhaps much too long,
 * and several points became clear.
 *
 * 1) makefile maintenance is a mess.
 * 2) it requires private headers, such as this one, to enforce even a moderate level
 *      of separation.  These are a pain to understand, IMHO.
 * 3) static functions are p nice and not to be taken lightly.  Ditto static extern vars.
 * 4) most of the successful libraries I looked at actually had many functions per file.
 *
 * Thus, I have decided Oly shall have sensible groups of functions in a single file.
 */
struct oly_struct {
    const OChar             *resource_dir;
    const OChar             *program_name;
    OlyResource             *data;
    OlyState                *state;
    OlyConfig               *config;
};

END_C_DECLS
#endif /* SRC_PVT_OLY_H */

