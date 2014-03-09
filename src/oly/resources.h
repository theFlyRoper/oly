/* oly/resources.h -- user message storage and delivery GPL2+ {{{
  
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

#ifndef SRC_OLY_MESSAGES_H
#define SRC_OLY_MESSAGES_H 1

#include "oly/common.h"
#include "oly/olytypes.h"

struct OlyResource_struct;
typedef struct OlyResource_struct OlyResource;
/* TODO: 
 * ICU has some really cool data loading capabilities, which should not be
 * hard to implement here.  Read about them here:
 * http://userguide.icu-project.org/icudata#TOC-How-Data-Loading-Works
 */
extern OlyResource *new_resource(const char *name, const char *locale, const char *charset);
extern OlyStatus    open_resource(OlyResource *res, char *res_dir, OlyStatus *status);
extern void         close_resource(OlyResource *res);
extern OChar       *cstr_to_ostr(OChar *o, const char *c, OlyStatus *status);
extern char        *ostr_to_cstr(char *c, const OChar *o, OlyStatus *status);

#endif /* SRC_OLY_MESSAGES_H */

