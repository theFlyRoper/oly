/* incl-yaml_to_sqlite.c - basic interface functions for yaml_to_sqlite. Intended to be included.  License GPL2+ {{{
 * Copyright (C) 2014 Oly Project
 * Permission is hereby granted, free of charge, to any person obtaining a copy of 
 * this software and associated documentation files (the "Software"), to deal in 
 * the Software without restriction, including without limitation the rights to 
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
 * of the Software, and to permit persons to whom the Software is furnished to do 
 * so, subject to the following conditions:
 * 
 *  The above copyright notice and this permission notice shall be included in 
 *  all copies or substantial portions of the Software.
 *  
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 * }}} */

OlyStatus
get_options(int argc, char **argv)
{
    OlyStatus status = OLY_OKAY;
    char c;
    /* y = YAML file, q = sqlite file, h = help, v = version */
    while ((c = getopt(argc, argv, "y:q:hv")) != -1) 
    {
        switch (c) 
        {
        case ('h'):
            status = OLY_WARN_SHOW_HELP;
            break;
        case ('v'):
            status = OLY_WARN_SHOW_VERSION;
            break;
        case ('y'):
            yaml_file = optarg;
            break;
        case ('q'):
            sqlite_file = optarg;
            break;
        default:
            status = OLY_ERR_BADARG;
            break;
        }
    }
    return status;
}

static void
pick_up_phone_booth_and_die( OlyStatus status )
{
            u_fprintf(u_stderr, "ERROR: ");
            u_fprintf_u(u_stderr, get_errmsg(status));
            u_fprintf(u_stderr, " Exiting...\n");
            exit(EXIT_FAILURE);
}

static void 
print_help(void)
{
    usage();
    u_fprintf(u_stdout,"A prototype to test buffering approaches between a \ncollection data source (YAML) and a table data source (sqlite 3).\n");
    u_fprintf(u_stdout,"Options:\n");
    u_fprintf(u_stdout,"\t\t-h\t\tShow this help message.\n");
    u_fprintf(u_stdout,"\t\t-v\t\tShow the program version.\n");
    u_fprintf(u_stdout,"\t\t-y\t\tyaml file to use.\n");
    u_fprintf(u_stdout,"\t\t-q\t\tsqlite file to use.\n");
    exit(EXIT_SUCCESS);
}

static void 
usage(void)
{
    u_fprintf(u_stdout, "%S: [OPTIONS]\n", 
           get_program_name());
}

static void 
print_version(void)
{
    u_fprintf(u_stdout, "%S: Version 1.0, 3-21-2014 - copyright (C) Oly Project\n", 
           get_program_name());
    u_fprintf(u_stdout, "\tLicensed according to the terms of the GNU general public\n");
    u_fprintf(u_stdout, "\tlicense, version 2 or (at your option) any later version.\n");
    u_fprintf(u_stdout, "\tThis program is provided in the hopes that it will be useful, but\n");
    u_fprintf(u_stdout, "\tWITHOUT ANY WARRANTY, to the extent permitted by law.\n\n");
    u_fprintf(u_stdout, "\tVisit %s for precise details on authorship.\n", PACKAGE_URL);
}


