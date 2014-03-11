/* init_oly.c -- perform initial cleaning of environment and initialize ICU  License GPL2+ {{{
 * Copyright (C) 2012 Oly Project
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

#include <pwd.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <errno.h>
#include <paths.h>
#include <libgen.h>

#include "oly/core.h"
#include "pvt_core.h"

OlyStatus
init_oly(Oly *oly, char *prog, char *datadir, char *charset, char *locale)
{
    OlyStatus       status = OLY_OKAY;
#ifdef HAVE_UNICODE_USTDIO_H
    UErrorCode       u_status = U_ZERO_ERROR; 
#endif /* HAVE_UNICODE_USTDIO_H */
    OChar           *program_mover = NULL, *resource_dir = XCALLOC(OChar,
                     (strlen(datadir)+1));
    int32_t          len = 0;
    OlyResource     *oly_init_resource = NULL;
    OlyState        *oly_init_state = NULL;
    
    assert(program_name == NULL && prog != NULL && datadir != NULL);
    /* aborts if encounters unusual states or unclosable files */
    clean_io_open();
    
    if ( cleanenv() != OLY_OKAY )
    {
        abort();
    }

#ifdef HAVE_UNICODE_USTDIO_H
    /* Initialize ICU */
    u_init(&u_status);
    if (U_FAILURE(u_status)) 
    {
        printf("Could not initialize ICU, error: %s. Exiting...\n",
                u_errorName(u_status));
        exit(EXIT_FAILURE);
    }
#endif /* HAVE_UNICODE_USTDIO_H */
    /* program name get. */
    len = strlen(basename(prog));
    if (len == 0)
    {
        printf("Program name not found. input: %s, basename: %s. Exiting...\n",
                prog, basename(prog));
        exit(EXIT_FAILURE);
    }
    program_mover = (UChar *)xcalloc(strlen(basename(prog)), sizeof(OChar));
#ifdef HAVE_UNICODE_USTDIO_H
    u_uastrncpy(program_mover, basename(prog), len);
#endif /* HAVE_UNICODE_USTDIO_H */
    program_name = (const OChar *)program_mover;
    if (set_resource_dir(datadir, &status) != OLY_OKAY)
    {
        printf("Could not set resource dir, error: %i\n", status);
        exit(EXIT_FAILURE);
    }
    if ( init_locale(&locale, &status) != OLY_OKAY )
    {
        printf("Init: init_locale failed. Err: %i\n", status);
        exit(EXIT_FAILURE);
    }
    if (init_charset (&charset, &status) != OLY_OKAY) 
    {
        printf("Init: init_charset failed. Err: %i\n", status);
        exit(EXIT_FAILURE);
    }

    oly_init_resource = new_resource( locale, charset );

    if ( open_resource(oly_init_resource, datadir, &status) != OLY_OKAY) 
    {
        printf("Init: could not open resource.  %i\n", status);
        exit(EXIT_FAILURE);
    }

    oly_init_state = new_state( oly_init_resource );
    /* u_stderr, u_stdout, u_stdin */
    init_io(locale, charset);
    
    atexit (close_oly);
    
    oly->resource_dir   = u_uastrcpy(resource_dir, locale);
    oly->data           = oly_init_resource;
    oly->state          = oly_init_state;

    return status;
}




