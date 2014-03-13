/* oly/state.h -- Success-failure tracking and dispatching GPL2+ {{{
  
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
#include "oly/olytypes.h"
#include "oly/resources.h"

BEGIN_C_DECLS

struct OlyState_struct;
typedef struct OlyState_struct OlyState;

/* this struct is in use but as of 3-8-2014 only display is implemented. */
typedef enum OlyStateWarningAction_enum {
    DISCARD = 0,
    HANDLE_OR_DISCARD = 1,
    WARNING_SUMMARIZE = 2,
    WARNING_LOG_ONLY = 3,
    WARNING_DISPLAY_ONLY = 4,
    WARNING_LOG_AND_DISPLAY = 5
} OlyStateWarningAction;

/* Oly will always try to handle errors once an error handler exists.
 * If this is set higher, it will do other things as well. */
typedef enum OlyStateErrorAction_enum {
    HANDLE_ONLY = 0,
    ERROR_SUMMARIZE = 1,
    ERROR_LOG_ONLY = 2,
    ERROR_DISPLAY_ONLY = 3,
    ERROR_LOG_AND_DISPLAY = 4
} OlyStateErrorAction;

typedef enum OlyStateUrgency_enum {
    NO_ACTION = 0,
    WARNING_ACTION = 1, /* only do the action for this warning. */
    ERROR_ACTION = 2,   /* only do the action for this error. */
    /* OLY_HANDLE = 2,       send to the handler function. Will do later. */
    STOP_ROW = 4,
    STOP_THREAD = 5,
    STOP_PROCESS = 6,   /* kill the process/script associated with the error. */
    FATAL_ERROR = 7   /* kills the program. */
} OlyStateUrgency;


OlyState *new_state( OlyResource *master );
END_C_DECLS

#endif /* !OLY_STATE_H */

