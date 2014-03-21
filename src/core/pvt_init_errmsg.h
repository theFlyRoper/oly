/* pvt_init_errmsg.h -- private data source definitions.  License GPL2+ {{{
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
 * }}} */

#ifndef PVT_INIT_ERRMSG_H
#define PVT_INIT_ERRMSG_H 1

#include "oly/data_source.h"
#include "oly/node.h"
#include "data_source/oly_yaml.h"


BEGIN_C_DECLS

/* function to call from the init subsystem to initialize the error handler. */

extern OlyStatus init_errmsg(Oly *oly);

END_C_DECLS

#endif /* PVT_INIT_ERRMSG_H */

