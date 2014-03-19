/* data_source.c - Oly data source dynamic loader License GPL 2+{{{
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
#include <sys/types.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <yaml.h>

#include <ltdl.h>

#define OLY_DATA_SOURCE_DIR ".olyrc/data_source"
#define DIR_SEPARATOR "/"

OlyDataSource * 
load_ds (const char *name, OlyStatus *status)
{
    lt_dlhandle     ds;
    unsigned int    ltdl_status = 0;
    char            char_err = NULL;


    last_error = NULL;

    module = lt_dlopenext (name);

    if (module)
        {
        builtin_table = (Builtin*) lt_dlsym (module, "builtin_table");
        syntax_table = (Syntax *) lt_dlsym (module, "syntax_table");
        if (!builtin_table && !syntax_table)
            {
            lt_dlclose (module);
            last_error = no_builtin_table_error;
            module = NULL;
            }
        }

    if (module)
        {
        ModuleInit *init_func
            = (ModuleInit *) lt_dlsym (module, "module_init");
        if (init_func)
            (*init_func) (sic);
        }

    if (module)
        {
        SyntaxFinish *syntax_finish
            = (SyntaxFinish *) lt_dlsym (module, "syntax_finish");
        SyntaxInit *syntax_init
            = (SyntaxInit *) lt_dlsym (module, "syntax_init");

        if (syntax_finish)
            sic->syntax_finish = list_cons (list_new (syntax_finish),
                                            sic->syntax_finish);
        if (syntax_init)
            sic->syntax_init = list_cons (list_new (syntax_init),
                                        sic->syntax_init);
        }

    if (module)
        {
        if (builtin_table)
            status = builtin_install (sic, builtin_table);

        if (syntax_table && status == SIC_OKAY)
            status = syntax_install (sic, module, syntax_table);

        return status;
        }

    last_error = lt_dlerror();
    if (!last_error)
        last_error = module_not_found_error;

    return SIC_ERROR;
}

int
data_source_init (void)
{
    static int initialised = 0;
    int errors = 0;

    /* Only perform the initialisation once. */
    if (!initialised)
    {
        /* ltdl should use the same mallocation as us. */
        lt_dlmalloc = (lt_ptr_t (*) (size_t)) omalloc;
        lt_dlfree = (void (*) (lt_ptr_t)) ofree;

        /* Make sure preloaded data_sources are initialised. */
        LTDL_SET_PRELOADED_SYMBOLS();

        last_error = NULL;
        errors = lt_dlinit();
        /* Set up the data_source search directories. */
        if (errors == 0)
        {
            const char *path = ocalloc((strlen(getenv ("HOME"))+1)
                    + (strlen(OLY_DATA_SOURCE_DIR)+1));
            strcpy(path, getenv("HOME"));
            strcat(path, DIR_SEPARATOR);
            strcat(path, OLY_DATA_SOURCE_DIR);

            if (path != NULL)
            {
                errors = lt_dladdsearchdir(path);
            }
        }

        if (errors == 0)
        {
            errors = lt_dladdsearchdir(DATASOURCEDIR);
        }

        if (errors != 0)
        {
            last_error = lt_dlerror ();
        }

        ++initialised;

        return errors ? OLY_ERROR : OLY_OKAY;
    }

    last_error = multi_init_error;
    return OLY_ERROR;
}

