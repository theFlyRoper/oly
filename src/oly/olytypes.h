/* olytypes.h -- typedef for OlyStatus enum License GPL2+ {{{
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

#ifndef OLY_OLYTYPES_H
#define OLY_OLYTYPES_H 1

/* main oly structure. details in src/pvt_core.h */
struct  oly_struct; 
typedef struct oly_struct Oly;

/* OlyStatus type:
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
typedef enum OlyStatus_enum {
    /* OLY_STATUS_OFFSET helps map these internal codes into the external i18n resource
     * files.
     */
    OLY_STATUS_OFFSET=4,
    OLY_WARN_REINIT=-4,
    OLY_WARN_ERROR_NOT_FOUND=-3,
    OLY_WARN_LIB=-2,
    OLY_WARN_EXIT=-1,
    OLY_OKAY=0,
    OLY_ERR_UNSPECIFIED=1,
    OLY_ERR_SYS=2,
    OLY_ERR_LIB=3,
    OLY_ERR_INIT=4,
    OLY_ERR_NOMEM=5,
    OLY_ERR_NOPWD=6,
    OLY_ERR_NOUSER=7,
    OLY_ERR_FILEIO=8,
    OLY_ERR_READHEX=9,
    OLY_ERR_BADARG=10
} OlyStatus;

typedef enum OlyBoolean_enum {
    OLY_TRUE=0,
    OLY_FALSE=-1
} OlyBoolean;

/* OFILE IO */
extern OFILE *u_stderr;
extern OFILE *u_stdout;
extern OFILE *u_stdin;

/* globals */
extern const OChar *program_name; 
extern char **environ; 
extern Oly *oly;

#endif /* !OLY_OLYTYPES_H */

