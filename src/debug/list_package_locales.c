/* list_package_locales.c - Debug function to print available locales. {{{
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

#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif
#include "common.h"

#include <unicode/ustdio.h>
#include <unicode/uloc.h>
#include <unicode/ures.h>

#include <sys/types.h>
#include <unistd.h>

void list_package_locales(const char *package_name)
{ 
  UEnumeration   *list; 
  UErrorCode      u_status = U_ZERO_ERROR;
  char           *item = NULL;
  int32_t         len = NULL;

  int       i = 0;
  list = ures_openAvailableLocales(package_name, &u_status);
  if (U_FAILURE(u_status)) {
    fprintf(stderr, "Can not find package %s locales. Err: %s\n",
        package_name, u_errorName(u_status));
    exit(1);
  }

  while ((item = uenum_next(list, len, &u_status)) != NULL) {
    printf("%s ", item);
  }
}

