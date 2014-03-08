/* resources_internal.h -- user message storage and delivery GPL2+ {{{
  
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

#include "oly/resources.h"
/* internal API */
#ifndef SRC_RESOURCES_INTERNAL_H
#define SRC_RESOURCES_INTERNAL_H 1
struct oly_resource_struct
{
    char                    *name;
    char                    *locale;
    char                    *charset;
    int                      array_offset;
    resource_data           *resource;
};
#endif /* SRC_RESOURCES_INTERNAL_H */

