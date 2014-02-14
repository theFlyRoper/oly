/* error.h -- display formatted error diagnostics of varying severity licence GPL2+ {{{
  
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

#ifndef OLY_ERROR_H
#define OLY_ERROR_H 1

BEGIN_C_DECLS

/* Oly_Status type:
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

typedef enum oly_status {
    OLY_EOF=-5,
    OLY_EXIT=-4,
    OLY_CONTINUE=-3,
    OLY_BREAK=-2,
    OLY_INCOMPLETE=-1,
    OLY_OKAY=0,
    OLY_ERR_ICU=1,
    OLY_ERR_UNSPECIFIED=2,
    OLY_ERR_INIT=3,
    OLY_ERR_NOPWD=4,
    OLY_ERR_NOARGS=6
} Oly_Status;

/* macro to catch Oly errors */
#define O_FAILURE (x) ((x)>OLY_OKAY)

extern UFILE *u_stderr;
extern UFILE *u_stdout;
extern UFILE *u_stdin;
extern const OChar *program_name;
extern UResourceBundle *OlyResources;

extern void oly_warning      (const OChar *message);
extern void oly_error        (const OChar *message);
extern void oly_fatal        (const OChar *message);

END_C_DECLS

#endif /* !OLY_ERROR_H */

