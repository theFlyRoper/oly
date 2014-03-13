/* oly/config.h - Basic functions, IO and other stuff. License GPL2+ {{{
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

#ifndef SRC_OLY_LOAD_CONFIG_H
#define SRC_OLY_LOAD_CONFIG_H 1

#include "oly/common.h"
#include "oly/state.h"
#include "oly/resources.h"

BEGIN_C_DECLS

typedef struct OlyConfig_struct OlyConfig;

typedef void (*OlyHandler)(char *input);
extern OlyConfig *load_config( void );

struct OlyConfig_struct {
    OChar *key;
    OChar *value;
    OlyHandler  handle;    
    OlyConfig *next;
};


END_C_DECLS

#endif /* SRC_OLY_LOAD_CONFIG_H */

