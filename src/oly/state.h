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
    OLY_WARN_ERROR_NOT_FOUND=-6,
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
    OLY_ERR_NOUSER=7,
    OLY_ERR_FILEIO=8,
    OLY_ERR_READHEX=9
} oly_status;

typedef struct oly_state_t *state_p;
#ifdef HAVE_UNICODE_URES_H
typedef UErrorCode liberror_num;
#endif /* HAVE_UNICODE_URES_H */

typedef enum oly_state_behavior_t {
    DISCARD = 0,
    SUMMARIZE = 1
    LOG_ERRORS = 2,
    DISPLAY_ERRORS = 3,
    LOG_AND_DISPLAY_ERRORS = 4,
    LOG_ALL = 5,
    DISPLAY_ALL = 6,
    VERBOSE = 6
} oly_state_behavior;

typedef enum oly_state_urgency_t {
    NO_ACTION = 0,
    WARN_ONLY = 1,      /* only perform the state_behavior for this error/warning. */
    HANDLE = 2,         /* send to the handler function. */
    STOP_ROW = 3,
    STOP_THREAD = 4,
    STOP_PROCESS = 5,   /* kill the process/script associated with the error. */
    FATAL_ERROR = 6    /* kills the program. */
} oly_state_urgency;

typedef struct oly_state_t {
  oly_status     status;               /* status for instance of oly_state */
  liberror_num   lib_status;
  ochar         *location;             /* where state just came from. */
  ochar         *message;              /* ochar holding the message */
  int           (*handler)(void *);    /* single argument handler function */
} oly_state;

oly_status init_state(oly_state *s);
oly_status set_status(oly_state *state, const oly_status status);
oly_status get_status(oly_state *state);
oly_status get_default_locale (char *locale, oly_status *status); 
extern void oly_warning      (const ochar *message);
extern void oly_error        (const ochar *message);
extern void oly_fatal        (const ochar *message);

END_C_DECLS

#endif /* !OLY_STATE_H */

