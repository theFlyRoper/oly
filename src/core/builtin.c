/* builtin.c -- installing and removing oly builtin commands {{{
   Copyright (C) Oly Project
  
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

#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif

#include <unicode/ustdio.h>

#include "oly/common.h"
#include "oly/state.h"
#include "oly/syntax.h"
#include "oly/list.h"
#include "oly/oly.h"
#include "oly/builtin.h"
#include "oly/eval.h"

struct builtintab {
  struct builtintab *next;
  Builtin *table;
};

int
builtin_install (Oly *oly, Builtin *table)
{
  int status = OLY_ERR_UNSPECIFIED;
  
  if (table)
    {
      BuiltinTab *new = XMALLOC (BuiltinTab, 1);
      new->next = oly->builtins;
      new->table = table;
      oly->builtins = new;
      status = OLY_OKAY;
    }

  return status;
}

int
builtin_remove (Oly *oly, Builtin *table)
{
  int status = OLY_ERR_UNSPECIFIED;
  
  if (oly->builtins && table)
    {
      BuiltinTab *stale = NULL;

      if (oly->builtins->table == table)
	{
	  stale = oly->builtins;
	  oly->builtins = oly->builtins->next;
	  status = OLY_OKAY;
	}
      else
	{
	  BuiltinTab *p;
	  
	  for (p = oly->builtins; p->next; p = p->next)
	    if (p->next->table == table)
	      {
		stale = p->next;
		p->next = p->next->next;
		status = OLY_OKAY;
		break;
	      }
	}

      XFREE (stale);
    }
  
  return status;
}

Builtin *
builtin_find (Oly *oly, const char *name)
{
  if (oly->builtins)
    {
      BuiltinTab *p;
      for (p = oly->builtins; p; p = p->next)
	{
	  int i;
	  for (i = 0; p->table[i].name; ++i)
	    if (strcmp (p->table[i].name, name) == 0)
	      return &p->table[i];
	}
    }

  return NULL;
}
