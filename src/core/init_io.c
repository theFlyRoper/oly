/* init_io.c - cleans and returns the u_std* global streams. License GPL2+ {{{
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
#include "oly/core.h"
#include "pvt_core.h"


void
init_io(const char *locale, const char *codepage) 
{
    char    err_buffer[BUFSIZ];
    int     errnum=0;

    assert((u_stderr == NULL) && (u_stdin == NULL) && (u_stdout == NULL));
    u_stderr=u_finit(stderr, locale,  codepage);
    if(!u_stderr) {
        if ( strerror_r( errnum, err_buffer, BUFSIZ ) == 0 ) {
            printf("%s", err_buffer);
        }
        exit(1);
    }
    u_stdout = u_finit(stdout, locale, codepage);
    if(!u_stdout) {
        if ( strerror_r( errnum, err_buffer, BUFSIZ ) == 0 ) {
            printf("%s", err_buffer);
        }
        exit(1);
    }
    u_stdin = u_finit(stdin, locale, codepage);
    if(!u_stdin) {
        if ( strerror_r( errnum, err_buffer, BUFSIZ ) == 0 ) {
            printf("%s", err_buffer);
        }
        exit(1);
    }
}

