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

#include "oly/core.h"

#ifndef OLY_STATE_H
#define OLY_STATE_H 1

BEGIN_C_DECLS

typedef struct oly_state_t *state_p;

typedef struct oly_state_t {
  oly_status  status;               /* status for instance of oly_state */
  ochar      *message;              /* ochar holding the message */
  oly_locinfo    *i18n;             /* localizing info struct */
  int        (*handler)(void *);    /* single argument handler function */
  void       *support;              /* holds pointer to supporting data. */
} oly_state;

extern void oly_warning      (const ochar *message);
extern void oly_error        (const ochar *message);
extern void oly_fatal        (const ochar *message);

oly_status init_oly_state(oly_state *s);

END_C_DECLS

#endif /* !OLY_STATE_H */

