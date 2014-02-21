/* oly.h -- data source functions. {{{
 * Copyright (C) 2014 Oly Project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 * }}} */

#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif

#ifndef SRC_DATA_SOURCE_H
#define SRC_DATA_SOURCE_H 1

BEGIN_C_DECLS

typedef enum oly_ds_direction {
  IN, OUT, INOUT
} OlyDSDirection;

Oly_Status oly_get_ds(OlyDataSource *ds, 
    ochar *ds_type, 
    OlyDSDirection ds_direct,
    ochar **ds_args);

END_C_DECLS

#endif /* SRC_DATA_SOURCE_H */

