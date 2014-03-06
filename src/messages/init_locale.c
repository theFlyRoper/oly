/* init_locale.c - initialize a locale object {{{
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

#include <sys/stat.h>
#include "oly/core.h"
#include "oly/resources.h"
/* if *locale_str is null, fills in the oly_locale info with a default. */

oly_status
init_locale(const char *locale_str, oly_locale *locale, oly_status *status)
{
    oly_status status = OLY_OKAY;
    if (locale_str == NULL) 
    {
        
    }
#ifdef HAVE_UNICODE_URES_H
#endif /* HAVE_UNICODE_URES_H */
    return status;
}

