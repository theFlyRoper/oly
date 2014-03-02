/* get_i18n_errmsg.c - get the ochar * error string for formatting. License GPL2+ {{{
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

#include "oly/common.h"
#include "oly/core.h"
#include <stdarg.h>

oly_status
get_i18n_errmsg(ochar **message, const oly_status oly_errno)
{
    int32_t message_len = 0;
    oly_status return_status = OLY_OKAY;
#ifdef HAVE_UNICODE_URES_H
#endif
    return return_status;
}
