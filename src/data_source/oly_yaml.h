/* oly_yaml.h -- data source functions for yaml {{{

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

#ifndef SRC_OLY_YAML_H
#define SRC_OLY_YAML_H 1

#include "oly/data_source.h"
#include <stdbool.h>

BEGIN_C_DECLS

/* YAML is a superset of JSON, and libyaml handles JSON correctly, but technically 
 * they are different data sources. 
 */

extern OlyStatus init_yaml( OlyDataSource *ds ) ;
extern void load_yaml( OlyStatus *status , OlyDataSource *ds );

END_C_DECLS

#endif /* SRC_OLY_YAML_H */


