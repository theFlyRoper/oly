/* oget_home.c - Oly function to retrieve home dir. License GPL2+ {{{
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

#include <unicode/ustdio.h>

#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#include "oly/common.h"
#include "oly/error.h"
#include "oly/loader.h"

char *
oget_home (struct passwd *pwd) {
    char *home = (char *)xmalloc((strlen(pwd->pw_dir)) + sizeof("HOME=") - 1);
    strcpy(home, "HOME=");
    strcat(home, pwd->pw_dir);
    return home;
}

