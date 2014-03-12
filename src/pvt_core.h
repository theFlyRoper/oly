/* pvt_core.h -- Internals for oly.  GPL2+ {{{
  
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
/* internal API */
#ifndef SRC_PVT_OLY_H
#define SRC_PVT_OLY_H 1
BEGIN_C_DECLS
struct oly_struct {
    const OChar             *resource_dir;
    const OChar             *program_name;
    OlyResource             *data;
    OlyState                *state;
    OlyConfig               *config;
};

/* holds the cleaned environment for use with getenv() */
extern void         clean_io_open(void);
extern OlyStatus    cleanenv(void);
extern void         close_oly(void);
extern void         init_io(const char *locale, const char *codepage);

END_C_DECLS
#endif /* SRC_PVT_OLY_H */

