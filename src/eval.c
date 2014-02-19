/* eval.c -- create and dispatch command vectors {{{
  
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
#include "oly/syntax.h"
#include "oly/list.h"
#include "oly/oly.h"
#include "oly/builtin.h"
#include "oly/eval.h"

static void bufferin_init  (BufferIn *in, OChar *command);
static void bufferout_init (BufferOut *out, size_t lim);

int
eval (Oly *oly, Tokens *tokens)
{
  Builtin *builtin;
  
  builtin = builtin_find (oly, tokens->argv[0]);

  if (!builtin)
    builtin = builtin_find (oly, "unknown");

  if (!builtin)
    {
      oly_result_clear (oly);
      oly_result_append (oly, "command \"");
      oly_result_append (oly, tokens->argv[0]);
      oly_result_append (oly, "\" not found");
      return OLY_ERR_UNSPECIFIED;
    }

  /* The table expresses valid counts of arguments, not including
     the command iteslf, hence the `-1'.  */
  if (tokens->argc -1 < builtin->min ||
      (tokens->argc -1> builtin->max && builtin->max != -1))
    {
      oly_result_set (oly, "wrong number of arguments", -1);
      return OLY_ERR_UNSPECIFIED;
    }

  return (*builtin->func) (oly, tokens->argc, tokens->argv);
}

int
untokenize (Oly *oly, OChar **pcommand, Tokens *tokens)
{
  OChar *command = NULL;
  int i, off;

  if (tokens->lim < 1)
    {
      for (tokens->lim = i = 0; tokens->argv[i]; ++i)
	tokens->lim += 1 + strlen (tokens->argv[i]);
    }

  command = XMALLOC ( OChar, tokens->lim);
  
  for (off = i = 0; tokens->argv[i]; ++i)
    {
      sprintf (&command[off], "%s ", tokens->argv[i]);
      off += 1 + strlen (tokens->argv[i]);
    }
  command[off -1] = 0;
  
  *pcommand = command;

  return OLY_OKAY;
}

int
tokenize (Oly *oly, Tokens **ptokens, OChar **pcommand)
{
  BufferIn in;
  BufferOut out;
  List *head;
  int status = OLY_CONTINUE;

  bufferin_init (&in, *pcommand);
  bufferout_init (&out, in.buf.lim);

  /* Perform any user initialisation for syntax readers. */
  for (head = oly->syntax_init; head; head = head->next)
    (*(SyntaxInit *) head->userdata) (oly);

  /* Dispatch to handlers by syntax class of character, or
     simply copy from input to output by default. */
  while (status == OLY_CONTINUE)
    {
      SyntaxHandler *handler = syntax_handler (oly, in.buf.start[in.buf.i]);

      if (handler)
	status = (*handler) (oly, &in, &out);
      else
	out.buf.start[out.buf.i++] = in.buf.start[in.buf.i++];
    }

  /* Perform any client finalisation for syntax reader. */
  for (head = oly->syntax_finish; head; head = head->next)
    (*(SyntaxFinish *) head->userdata) (oly, &in, &out);
	
  {
    /* Can't fill ARGV on the fly incase BUF moved during realloc. */
    Tokens *tokens = XMALLOC (Tokens, 1);
    
    tokens->argv = XMALLOC ( OChar *, 1 + out.offc);

    for (tokens->argc = 0; tokens->argc < out.offc; ++tokens->argc)
      tokens->argv[tokens->argc] = out.buf.start + out.offv[tokens->argc];

    tokens->argv[tokens->argc] = NULL;
    tokens->lim  = out.buf.lim;

    if (!*tokens->argv)
      XFREE(out.buf.start);
    XFREE (out.offv);

    *pcommand += in.buf.i;
    *ptokens = tokens;
  }

  return status;
}

static void
bufferin_init (BufferIn *in, OChar *command)
{
  in->buf.lim	= 1 + strlen (command);
  in->buf.start	= command;
  in->buf.i	= 0;

  in->start	= in->buf.start;
}

static void
bufferout_init (BufferOut *out, size_t lim)
{
  out->buf.lim	= lim;
  out->buf.start= XMALLOC ( OChar, lim);
  out->buf.i	= 0;

  out->offc	= 0;
  out->offv	= NULL;
  out->argind	= out->buf.i;
}

