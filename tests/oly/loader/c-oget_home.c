/* main for csv importer. License GPL2+ {{{
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
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>

#include "common.h"
#include "loader.h"

extern char **environ; 

int
main(void){
  int             status = 0;
  char            homebuf[100];
  uid_t           uid = getuid();
  struct passwd   *pwd;

  strcpy(homebuf,"HOME=");
  strcat(homebuf,getenv("HOME"));
  if ( ! (pwd = getpwuid(uid)) ){
      endpwent();
      return 1;
  }
  if (strncmp(homebuf,oget_home(pwd), sizeof(oget_home(pwd))) != 0)
    status = 1;
  
  endpwent();
  /* printf("%i\n", status); */
  return status;
}


