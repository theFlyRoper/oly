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

typedef enum oly_locale_type_t 
{
#ifdef HAVE_UNICODE_URES_H
    VALID_LOCALE  = ULOC_VALID_LOCALE,
    ACTUAL_LOCALE = ULOC_ACTUAL_LOCALE 	
#endif /* HAVE_UNICODE_URES_H */
} oly_locale_type;

typedef struct oly_locale_t {
    char            *name;
    oly_locale_type  locale_type;
    void            *object;
} oly_locale;

typedef enum oly_resource_type_t {
    COMPLEX_TABLE=0,
    COMPLEX_ARRAY=1,
    STRING_TABLE=2,
    STRING_ARRAY=3
} oly_resource_type;

struct oly_resource;

typedef struct oly_resource_t 
{
    ochar                *name;
    oly_resource_type     type;
    int                   array_offset;
    oly_locale            locale;
    resource_data        *resource;
} oly_resource;

oly_status init_resource_api(const char *dir, oly_status *status);
oly_status init_locale(const char *locale_str, oly_locale *locale, oly_status *status);
oly_status open_resource(const oly_locale *locale, oly_resource *resource, 
        oly_status *status);
oly_status close_resource(oly_resource *resource, oly_status *status);

char *get_default_locale (oly_status *status) ;

/* internal API */
#ifdef SRC_OLY_MESSAGES_INTERNAL_H

#endif /* SRC_OLY_MESSAGES_INTERNAL_H */
#endif /* SRC_OLY_MESSAGES_H */

