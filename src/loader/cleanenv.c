/* cleanenv.c -- perform initial cleaning of environment  License GPL2+ {{{
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
 * 
 * Adapted from the secure programming cookbook, see
 * doc/bibliography.wiki for details.
 * }}} */

#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif

#include <unicode/ustdio.h>
#include <unicode/ustring.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <assert.h>

#include "common.h"
#include "output.h"
#include "error.h"
#include "list.h"
#include "syntax.h"
#include "oly.h"

/* The following function(s) are
 * adapted from the Secure Programming 
 * Cookbook, John Viega and Matt Messier.
 * 2003, O'Reilly Press.
 */

/* These arrays are both null-terminated. 
 * was thinking of recording current user, but, enh.
 */

const char *
spc_restricted_environ[  ] = {
  "IFS= \t\n",
  0,            /* spot for path. */
  0,            /* spot for home. */
  0,            /* spot for username. */
  0
};
 
const char *
spc_preserve_environ[  ] = {
  "LC_ALL",
  "LANG",
  "TZ",
  0
};

int
cleanenv (Oly *oly) {
    int         i,status=OLY_OKAY;
    char        **new_environ, *path, *home, *username, *ptr;
    const char  *value, *var; 
    size_t path_size = 0,
           arr_size = 1, 
           arr_ptr = 0, 
           len, 
           new_size = 0;
 
    uid_t           uid;
    struct passwd   *pwd;
    uid = getuid();
    
    if ( ! (pwd = getpwuid(uid)) ){
        endpwent();
        status=OLY_ERR_NOUSER;
        return status;
    }

    path_size=(strlen(pwd->pw_dir));

    home = (char *)xmalloc(path_size + sizeof("HOME=") - 1);

    strcpy(home, "HOME=");
    strcat(home, pwd->pw_dir);   
 
    path_size=(strlen(pwd->pw_name));

    username = (char *)xmalloc(path_size + sizeof("USERNAME=") - 1);

    strcpy(username, "USERNAME=");
    strcat(username, pwd->pw_name);   
    
    endpwent();
    
       
    if ((path_size = confstr(_CS_PATH, NULL, 0)) > 0) {
        
        path = (char *)xmalloc(path_size + sizeof("PATH=") - 1);
   
        if (path == NULL) {
            status=OLY_ERR_NOPATH;
        }
   
        strcpy(path, "PATH=");
    
        if (confstr(_CS_PATH, path + sizeof("PATH=") - 1, path_size) > 0) {
            spc_restricted_environ[1] = path;
        }
        else {
            free(path);
            path = NULL;
        }
  
    }
    
    spc_restricted_environ[2] = home;
    spc_restricted_environ[3] = username;
  
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


