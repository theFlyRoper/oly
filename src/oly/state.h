/* state.h -- Check current state and display formatted warnings, errors GPL2+ {{{
  
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

#ifndef OLY_STATE_H
#define OLY_STATE_H 1

BEGIN_C_DECLS

/* oly_status type:
 * Defines states for Oly.
 * 
 * Negative numbers and zero are non-error states
 * or warning states. Positive numbers are error 
 * states. Starts at lowest negative number and goes 
 * up. 
 *
 * Standard return state is OLY_OKAY, which you should
 * test for after calling most functions. Use others as
 * appropriate. OLY_OKAY is equal to zero.
 */

typedef enum oly_status_t {
    OLY_EOF=-5,
    OLY_EXIT=-4,
    OLY_CONTINUE=-3,
    OLY_BREAK=-2,
    OLY_INCOMPLETE=-1,
    OLY_OKAY=0,
    OLY_ERR_UNSPECIFIED=1,
    OLY_ERR_SYS=2,
    OLY_ERR_LIB=3,
    OLY_ERR_INIT=4,
    OLY_ERR_NOMEM=5,
    OLY_ERR_NOPWD=6,
    OLY_ERR_NOUSER=7
} oly_status;

extern OFILE *u_stderr;
extern OFILE *u_stdout;
extern OFILE *u_stdin;
extern const ochar *program_name;
extern UResourceBundle *OlyResources;

extern void oly_warning      (const ochar *message);
extern void oly_error        (const ochar *message);
extern void oly_fatal        (const ochar *message);

END_C_DECLS

#endif /* !OLY_STATE_H */

