/* pvt_state.h -- internals for state GPL2+ {{{
  
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

#include "oly/state.h"
/* internal API */
#ifndef SRC_PVT_STATE_H
#define SRC_PVT_STATE_H 1
BEGIN_C_DECLS

struct OlyState_struct{
    OlyStatus        status;          /* status for instance of OlyState */
    LibErrorNum      lib_status;      /* library status for external
                                         errors. */
    size_t           msgbuf_size;     /* How big is the message buffer */
    OlyStateErrorAction     error_action;       /* what to do with an error. */
    OlyStateWarningAction   warning_action;     /* what to do with a warning */
    OChar           *msgbuf_start;  /* OChar holding the message */
    OChar           *msgbuf_end;    /* Where the current buffer ends */
    OlyResource     *messages;      /* resource holding messages for state.*/
};

extern OlyStatus reset_liberror(OlyState *s);
extern OlyBoolean message_buffer_has_room(OlyState *s);

END_C_DECLS
#endif /* SRC_PVT_STATE_H */

