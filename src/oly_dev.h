/* oly_dev.h -- Debug functions {{{
  
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

#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif
#include "common.h"

#ifndef SRC_OLY_DEV_H
#define SRC_OLY_DEV_H 1

BEGIN_C_DECLS

extern void list_icu_countries(void);
extern void list_icu_langs(void);
extern void list_package_locales(const char *package_name);

END_C_DECLS

#endif /* SRC_OLY_DEV_H */
