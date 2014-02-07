/* oly_src.h  License GPL2+ {{{
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


#ifndef OLY_SRC_H
#define OLY_SRC_H 1

#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#else
#  define PACKAGE "PLACEHOLDER"
#  define VERSION "-1"
#endif

BEGIN_C_DECLS
extern char **environ; 

/* Oly_Status type:
 * Defines states for Oly.
 * 
 * Positive numbers and zero are non-error states
 * or warning states. Negative numbers are error 
 * states. Starts at lowest negative number and goes 
 * up. 
 *
 * Standard return state is OLY_OKAY, which you should
 * test for after calling most functions. Use others as
 * appropriate. OLY_OKAY is equal to zero.
 */
typedef enum oly_status {
    OLY_ERR_NOPATH=-6,
    OLY_ERR_NOUSER=-5,
    OLY_ERR_UNICODE=-4,
    OLY_ERR_INIT=-3,
    OLY_ERR_NOARGS=-2,
    OLY_ERR_UNSPECIFIED=-1,
    OLY_OKAY=0,
    OLY_INCOMPLETE=1,
    OLY_BREAK=2,
    OLY_CONTINUE=3,
    OLY_EXIT=4,
    OLY_EOF=5
} Oly_Status;

END_C_DECLS

/** @end 2 }}} */

#endif /* OLY_SRC_H */


