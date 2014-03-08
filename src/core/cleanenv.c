/* cleanenv.c - functions to clean the local environment. License GPL2+ {{{
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

#include <pwd.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <errno.h>
#include <paths.h>
#include <libgen.h>

#include "oly/core.h"
#include "pvt_core.h"

/* get_home, cleanenv 
 * are adapted from the Secure Programming 
 * Cookbook, John Viega and Matt Messier.
 * 2003, O'Reilly Press, 
 * ISBN: 0-596-00394-3
 */

static char *get_home (struct passwd *pwd);

char *
get_home (struct passwd *pwd) 
{
    char *home = (char *)xmalloc((strlen(pwd->pw_dir)) + sizeof("HOME=") - 1);
    strcpy(home, "HOME=");
    strcat(home, pwd->pw_dir);
    return home;
}

/* These arrays are both null-terminated. 
 * was thinking of recording current user, but, enh.
 */

const char *
spc_restricted_environ[  ] = {
    0,            /* spot for home. */
    "IFS= \t\n",
    "PATH=" _PATH_STDPATH,
    0
};
 
const char *
spc_preserve_environ[  ] = {
    "LANGUAGE",
    "LC_MESSAGES",
    "LC_CTYPE",
    "LC_ALL",
    "LANG",
    "TZ",
    0
};


OlyStatus
cleanenv (void) {
    int         i,status=OLY_OKAY;
    char        **new_environ, *ptr;
    const char  *value, *var; 
    size_t arr_size = 1, arr_ptr = 0, len = 0, 
           new_size = 0;
 
    uid_t           uid = getuid();
    struct passwd   *pwd;
    
    if ( ! (pwd = getpwuid(uid)) ){
        endpwent();
        status=OLY_ERR_NOPWD;
        return status;
    }

    spc_restricted_environ[0] = get_home(pwd);
    endpwent();
  
    for (i = 0;  (var = spc_restricted_environ[i]) != 0;  i++)
    {
        new_size += strlen(var) + 1;
        arr_size++;
    }

    for (i = 0;  (var = spc_preserve_environ[i]) != 0;  i++)
    {
        if (!(value = getenv(var))) continue;
        new_size += strlen(var) + strlen(value) + 2;
        arr_size++;
    }

    new_size += (arr_size * sizeof(char *));
    if (!(new_environ = (char **)xmalloc(new_size))) 
        abort(  );
    new_environ[arr_size - 1] = 0;
    
    ptr = (char *)new_environ + (arr_size * sizeof(char *));
    for (i = 0;  (var = spc_restricted_environ[i]) != 0;  i++) 
    {
        new_environ[arr_ptr++] = ptr;
        len = strlen(var);
        memcpy(ptr, var, len + 1);
        ptr += len + 1;
    }
    for (i = 0;  (var = spc_preserve_environ[i]) != 0;  i++) {
        if (!(value = getenv(var))) continue;

        new_environ[arr_ptr++] = ptr;
        len = strlen(var);
        memcpy(ptr, var, len);
        *(ptr + len) = '=';
        memcpy(ptr + len + 1, value, strlen(value) + 1);
        ptr += len + strlen(value) + 2;
    }
    environ = new_environ;
    return status;
}


