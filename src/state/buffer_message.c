/* buffer_message.c - check the UErrorCode and holler if it is suboptimal. License GPL2+ {{{
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
#include <ctype.h>

#include "oly/state.h"
#include "oly/resources.h"
#include "pvt_state.h"

/*  OlyStateErrorAction   error_action;  what to do with an error. */
/*  OlyStateWarningAction   warning_action;   what to do with a warning */
OlyStatus 
buffer_message ( OlyState *state, OlyStateUrgency *urgency, 
        const OChar *message, ... )
{
    size_t      size_left = (msgbuf_size - 
            (size_t)((state->msgbuf_pos) - (state->msgbuf_pos)));
    size_t      msgbuf_size;     /* How big is the message buffer */
    OChar           *msgbuf_start;         /* OChar holding the message */
    OChar           *msgbuf_end;    /* Where the current buffer ends */
    return s->status;
}

