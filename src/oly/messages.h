/* messages.h -- user message storage and delivery GPL2+ {{{
  
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
#include "oly/common.h"
#include "oly/state.h"

#ifndef SRC_OLY_MESSAGES_H
#define SRC_OLY_MESSAGES_H 1

typedef oly_resource oly_messages;

struct Oly;

ochar *get_message(struct Oly *oly, 
        const oly_status input_status, ochar **args);

#endif /* SRC_OLY_MESSAGES_H */

