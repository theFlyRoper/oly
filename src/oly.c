/* oly.c -- create and maintain oly ADT {{{
  
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

#include "oly/common.h"
#include "oly/error.h"
#include "oly/list.h"
#include "oly/syntax.h"
#include "oly/oly.h"
#include "oly/builtin.h"
#include "oly/eval.h"

#define CHUNK_SIZE	256

static OlyState *olystate_find (Oly *oly, const OChar *key);

Oly *
oly_new (void)
{
  Oly *new = XCALLOC (Oly, 1);
  return new;
}

Oly *
oly_delete (Oly *stale)
{
  free (stale->result);
  free (stale);

  return 0;
}

int
olystate_set (Oly *oly, const OChar *key, void *value, 
	      void (*delete) (void *value))
{
  OlyState *state = olystate_find (oly, key);

  if (state)
    {
      if (state->delete)
	(*state->delete) (state->data);
    }
  else
    {
      state = XMALLOC (OlyState, 1);

      state->next = oly->state;
      oly->state  = state;

      state->key  = xstrdup (key);
    }

  state->data   = value;
  state->delete = delete;

  return 0;
}

void *
olystate_get (Oly *oly, const OChar *key)
{
  OlyState *state = olystate_find (oly, key);
  return state ? state->data : NULL;
}

int
olystate_clear (Oly *oly, const OChar *key)
{
  OlyState *state = oly->state;
  OlyState *stale = NULL;

  if (!state)
    return -1;

  if (wcscmp (state->key, key) == 0)
    {
      stale = state;
      oly->state = state->next;
    }
  else
    for (; state->next; state = state->next)
      if (wcscmp (state->next->key, key) == 0)
	{
	  stale = state->next;
	  state->next = stale->next;
	  break;
	}

  if (!stale)
    return -1;

  if (stale->delete)
    (*stale->delete) (stale->data);
  XFREE (stale->key);
  XFREE (stale);

  return 0;
}

Oly *
oly_result_clear (Oly *oly)
{
  oly->len = 0;
  if (oly->result)
    oly->result[0] = 0;
  else
    oly->result = 0;
  
  return oly;
}

const OChar *
oly_result_get (Oly *oly)
{
  return oly->result;
}

Oly *
oly_result_set (Oly *oly, const OChar *value, size_t len)
{
  if (len < 0) len = strlen (value);
  
  oly_result_clear (oly);
  
  if (len < CHUNK_SIZE)
    {
      oly->lim = CHUNK_SIZE;
      oly->result = XREALLOC ( OChar, oly->result, oly->lim);
    }
  
  return oly_result_append (oly, value);
}

Oly *
oly_result_append (Oly *oly, const OChar *value)
{
  size_t len;
  
  if (!value) return oly;

  len = strlen (value);
  if (oly->len + len >= oly->lim)
    {
      oly->lim += CHUNK_SIZE;
      oly->result = XREALLOC (OChar, oly->result, oly->lim);
    }

  strncpy (&oly->result[oly->len], value, len);
  oly->len += len;
  oly->result[oly->len] = 0;

  return oly;
}

static OlyState *
olystate_find (Oly *oly, const OChar *key)
{
  OlyState *state;

  for (state = oly->state; state; state = state->next)
    if (strcmp (state->key, key) == 0)
      break;

  return state;
}
	
