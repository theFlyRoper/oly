/* xmalloc.c -- memory management with out of memory reporting {{{
  
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.
   }}} */

#if HAVE_CONFIG_H
# include "olyconf.h"
#endif

#include <unicode/ustdio.h>
#include <sys/types.h>

#include "oly/common.h"
#include "oly/state.h"

void *
xmalloc (size_t num)
{
  void *new = malloc (num);
  if (!new)
    oly_fatal ("Memory exhausted");
  return new;
}

void *
xrealloc (void *p, size_t num)
{
  void *new;

  if (!p)
    return xmalloc (num);

  new = realloc (p, num);
  if (!new)
    oly_fatal ("Memory exhausted");

  return new;
}

void *
xcalloc (size_t num, size_t size)
{
  void *new = xmalloc (num * size);
  bzero (new, num * size);
  return new;
}

