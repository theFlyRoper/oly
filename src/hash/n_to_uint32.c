/* char_to_size32 - convert 8 chars into a single uint32_t and return it {{{
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

uint32_t char_to_size32(const unsigned char *c){
  /* converts 4 chars into a single sizeof(intval). */
  size_t    i = 0;
  uint32_t  res = 0;
  for (i = 0; (i < sizeof(uint32_t)); i += sizeof(char)) {
    res |= (c[i] << (CHAR_BIT * (sizeof(uint32_t) - i)));
  }
  return OLY_OKAY;
}
