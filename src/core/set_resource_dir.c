/* set_resource_dir.c -- set application i18n resource directory License GPL2+ {{{
 * Copyright (C) 2012 Oly Project
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

#include "oly/common.h"
#include "oly/core.h"

oly_status   set_resource_dir(const char *dir, oly_status *status)
{
    *status = OLY_OKAY;
#ifdef HAVE_UNICODE_USTDIO_H
    /* u_setDataDirectory tells ICU where to look for custom app data.  It is not needed
    * for the internal app data for ICU, which lives in a shared library. 
    */
    u_setDataDirectory(dir);
#endif /* HAVE_UNICODE_USTDIO_H */
    return *status;
}
