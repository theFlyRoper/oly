/* print_nint_as_hex.c: for N ints, print each as a hex, with no spaces. {{{
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

#include "oly/common.h"
#include <limits.h>

#include "oly/core.h"
#include "oly/hash.h"

void print_result(const char *c){
  int       i=0, limit = (OLY_HASH_SIZE/CHAR_BIT);
  for (i = 0; (i <limit ); i += sizeof(char)) {
    printf("%02x", (unsigned char)c[i]);
  }
  printf("\n");
}

