/* oly_mem_string.c - core functionality that is used throughout oly. License GPL 2+{{{
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
#include "sys/types.h"
#include "oly/core.h"
#include "pvt_core.h"

void *
omalloc (size_t num)
{
  void *new = malloc (num);
  if (!new)
  {
    u_fprintf_u(u_stderr, get_errmsg( OLY_ERR_NOMEM ));
    exit(EXIT_FAILURE);
  }
  return new;
}

void *
orealloc (void *p, size_t num)
{
  void *new;

  if (!p)
    return omalloc (num);

  new = realloc (p, num);
  if (!new)
  {
    u_fprintf_u(u_stderr, get_errmsg( OLY_ERR_NOMEM ));
    exit(EXIT_FAILURE);
  }
  return new;
}

void *
ocalloc (size_t num, size_t size)
{
  void *new = omalloc (num * size);
  memset(new, '\0', (num * size));
  return new;
}

char *
ostrdup (const char *string)
{
  return string ? strcpy (omalloc (strlen (string) + 1), string) : NULL;
}

/* If strerror returns NULL, we'll format the number into a static 
 * buffer.  */
#define ERRSTR_FMT "undocumented error #%d"
static char ostrerror_buf[sizeof ERRSTR_FMT + 20];
/* Like strerror, but result is never a null pointer.  */
char *
ostrerror (int errnum)
{
    char *errstr;
    strerror_r (errnum, ostrerror_buf, (sizeof(ERRSTR_FMT)+20) );

    errstr = ostrerror_buf;
  /* If `errnum' is out of range, result might be NULL.  We'll fix that.  */
  if (!errstr)
    {
      sprintf (ostrerror_buf, ERRSTR_FMT, errnum);
      errstr = ostrerror_buf;
    }

  return errstr;
}
#undef ERRSTR_FMT


