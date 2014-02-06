/* error.c -- display formatted error diagnostics of varying severity {{{
  
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

#include <unicode/ustdio.h> 

#include "common.h"
#include "error.h"

/** function definitions  */

static void error (int32_t exit_status,
                  const UChar *mode, 
                  const UChar *message);

static void
error (int32_t exit_status,
    const UChar *mode, const UChar *message)
{

  u_fprintf (u_stderr, "%s: %s: %s.\n", program_name, mode, message);

  if (exit_status >= 0)
    exit (exit_status);
}

void
oly_warning (const UChar *message)
{
  error (-1, "warning", message);
}

void
oly_error (const UChar *message)
{
  error (-1, "ERROR", message);
}

void
oly_fatal (const UChar *message)
{
  error (EXIT_FAILURE, "FATAL", message);
}



