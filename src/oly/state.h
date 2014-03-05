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

#include "oly/errors.h"

#ifndef OLY_STATE_H
#define OLY_STATE_H 1

BEGIN_C_DECLS
typedef struct oly_state_t *state_p;
#ifdef HAVE_UNICODE_URES_H
typedef UErrorCode liberror_num;
#endif /* HAVE_UNICODE_URES_H */
/* right now behavior and urgency do not do anything but I would like it if they did. */
typedef enum oly_state_behavior_t {
    DISCARD = 0,
    SUMMARIZE = 1,
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

typedef struct oly_state_t 
{
    oly_status     status;            /* status for instance of oly_state */
    liberror_num   lib_status;        /* library status for external errors. */
    ochar         *result;              /* ochar holding the message */
} oly_state;

oly_status init_state( oly_state *s );
oly_status close_state( oly_state *s );
oly_status set_status( oly_state *state, const oly_status status );
oly_status get_status( oly_state *state );
oly_status check_liberror       (oly_state *state);

/* these are not thread safe. All work with the BUFSIZ buffer in message. */
oly_status set_state_message    ( oly_state *state, const ochar *message, ... );
oly_status append_state_message ( oly_state *state, const ochar *message, ... );
oly_status clear_state_message  ( oly_state *state );
oly_status print_state_message  ( oly_state *state );
oly_status get_state_message    ( oly_state *state, ochar *msg );
oly_status clear_liberror(oly_state *s);
oly_status set_liberror(oly_state *s, int32_t err_val);
oly_status check_liberror(oly_state *s);
extern void oly_warning      (const ochar *message);
extern void oly_error        (const ochar *message);
extern void oly_fatal        (const ochar *message);

#ifdef SRC_OLY_STATE_INTERNAL_H
oly_status set_liberror_zero(oly_state *s);
#endif /* SRC_OLY_STATE_INTERNAL_H */

END_C_DECLS

#endif /* !OLY_STATE_H */

