/* open_ofile.c - locale-aware fopen wrapper with error checking. {{{
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

#include <sys/stat.h>
#include "oly/core.h"
#include "sys/types.h"

oly_status
open_ofile(OFILE **f, char *n, Oly *oly)
{
    char        errbuf[OLY_SMALL_BUFFER];
    fpos_t      pos;
    struct stat fstat;
    int         stat_err = stat(n, &fstat) ;

    if (stat_err != 0) && ((oly->messages) != NULL)
    {
        printf("placeholder for real message!\n");
    }
    else if (stat_err != 0)
    {
        printf("Open file error: %s\n", strerror_r(stat_err,
                    &errbuf, OLY_SMALL_BUFFER);
    } 
    else
    {
        
    }
    return status;
}

