/* syntax.c -- create and administer syntax tables
  
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
*/

#if HAVE_CONFIG_H
# include "olyconf.h"
#endif

#include <unicode/ustdio.h>

#include "common.h"
#include "error.h"
#include "syntax.h"
#include "list.h"
#include "oly.h"

struct syntaxnode {
  struct syntaxnode *next;
  SyntaxHandler *handler;
};

static struct syntaxnode *syntax_new (struct syntaxnode *head,
				      SyntaxHandler *handler);
static int syntaxelt_address_compare (List *element, void *match);

int
syntax_install (Oly *oly, Syntax *table)
{
  Syntax *table_ptr;

  /* Allocate some room for the syntax tables. */
  if (!oly->syntax)
    oly->syntax = XCALLOC (SyntaxTable*, 256);

  for (table_ptr = table; table_ptr->handler; ++table_ptr)
    {
      OChar *ch_ptr = table_ptr->ch;
      
      /* You can set the syntax habdler for ASCII '\0' by using
	 "" in the syntax table. */
      if (*ch_ptr == 0)
	oly->syntax[0] = syntax_new (oly->syntax[0], table_ptr->handler);

      /* Otherwise the handler is applied to each character in
	 the string of chars from the associated row. */
      for (; *ch_ptr; ++ch_ptr)
	oly->syntax[(int) *ch_ptr]
	  = syntax_new (oly->syntax[(int) *ch_ptr], table_ptr->handler);
    }

  return OLY_OKAY;
}
	
SyntaxHandler *
syntax_handler (Oly *oly, int ch)
{
  return oly->syntax[ch] ? oly->syntax[ch]->handler : NULL;
}

static struct syntaxnode *
syntax_new (struct syntaxnode *head, SyntaxHandler *handler)
{
  struct syntaxnode *new = XCALLOC (struct syntaxnode, 1);

  new->next    = head;
  new->handler = handler;

  return new;
}
