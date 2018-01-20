/* oly_sqlite3.h -- data source functions for sqlite3 {{{

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

#ifndef SRC_OLY_SQLITE3_H
#define SRC_OLY_SQLITE3_H 1

#include "oly/data_source.h"
#include <stdbool.h>

BEGIN_C_DECLS

extern OlyStatus open_sqlite3 ( OlyDataSource *ds );
extern OlyStatus close_sqlite3( OlyDataSource *ds );
extern OlyStatus get_from_sqlite3 ( OlyStatus *status, OlyDataSource *ds );
extern OlyStatus put_to_sqlite3   ( OlyStatus *status, OlyDataSource *ds );

END_C_DECLS

#endif /* SRC_OLY_SQLITE3_H */


