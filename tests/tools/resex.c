/* resex.c - resource bundle explorer {{{
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

#include "oly/common.h"
#include "oly/core.h"
#include "oly/oly_dev.h"
#include "oly/globals.h"

static void print_help(void);
static void usage(void);
static void print_version(void);
static void close_main(void);

/* MAIN */
int
main( int argc, char **argv ){
    char            *locale = NULL, *locdir=(char*)PKGDATADIR,
                    c, *find_me = NULL, *filename = OLY_RESOURCE, 
                    *progval = xstrdup(argv[0]);
    res_disp_flag   flag;
    UErrorCode      u_status  = U_ZERO_ERROR;
    Oly             *oly=(Oly *)xmalloc(sizeof(Oly));
    atexit (close_main);
    init_res_disp_flag(&flag);

    /* a = arrays, A = Aliases, b = binaries, d = search dir, e = everything,
     * f = find this, h = help, i = integers, l = locales, -L show only locale
     * s = strings, t = tables, v = version, V = vectors
     */
    while ((c = getopt(argc, argv, "aAbd:hil:Ln:stvV")) != -1) 
    {
        switch (c) 
        {
        case ('h'):
            flag.all = 0;
            flag.help = 1;
            break;
        case ('v'):
            flag.all = 0;
            flag.version = 1;
            break;
        case ('L'):
            flag.all = 0;
            flag.only_locale = 1;
            break;
        case ('l'):
            locale = optarg;
            break;
        case ('d'):
            locdir = optarg;
            break;
        case ('f'):
            find_me = optarg;
            break;
        case ('n'):
            filename = optarg;
            break;
        case ('a'):
            flag.all = 0;
            flag.arrays = 1;
            break;
        case ('A'):
            flag.all = 0;
            flag.aliases = 1;
            break;
        case ('b'):
            flag.all = 0;
            flag.binaries = 1;
            break;
        case ('i'):
            flag.all = 0;
            flag.integers = 1;
            break;
        case ('s'):
            flag.all = 0;
            flag.strings = 1;
            break;
        case ('t'):
            flag.all = 0;
            flag.tables = 1;
            break;
        case ('V'):
            flag.all = 0;
            flag.vectors = 1;
            break;
        default:
            flag.all = 0;
            flag.badopt = 1;
            break;
        }
    }
    if (init_oly(oly, progval, locdir, NULL, locale) != OLY_OKAY) {
        perror("Initialization failed\n");
    };
    if (flag.badopt == 1) 
    {
        printf("Error: unrecognized option.");
        usage();
        exit(EXIT_FAILURE);
    }
    else if (flag.help == 1)
    {
        usage();
        print_help();
        exit(EXIT_SUCCESS);
    }
    else if (flag.version == 1)
    {
        print_version();
        exit(EXIT_SUCCESS);
    }
    else if (flag.only_locale == 1)
    {
        printf("selected locale: %s\n", oly->data->locale);
        exit(EXIT_SUCCESS);
    }

    if (find_me == NULL) 
    {
        list_table_resources(oly->data->resource, &flag, 0);
    }
  
    if (U_FAILURE(u_status)) {
        exit(EXIT_FAILURE);
    } else {
        exit(EXIT_SUCCESS);
    }
}

static void 
close_main (void) 
{
    fclose (stdout); 
    fclose (stdin); 
}

static void 
print_help(void)
{
    usage();
    u_fprintf(u_stdout,"A tool to pry open icu .res / .dat files and browse them.\n");
    u_fprintf(u_stdout,"\tCommands:\n\t\t-d [SEARCH DIRECTORY]\n\t\t-f [THING TO FIND]\n");
    u_fprintf(u_stdout,"\t\t-l [LOCALE TO SEARCH]\n\t\t-n [FILE NAME]\n\n\tFlags:\n\t\t");
    u_fprintf(u_stdout,"-a\t\tshow names of arrays.\n\t\t-A\t\tshow names of Aliases.\n\t\t");
    u_fprintf(u_stdout,"-b\t\tbinaries\n\t\t");
    u_fprintf(u_stdout,"-i\t\tshow integer values.\n\t\t");
    u_fprintf(u_stdout,"-s\t\tshow string values\n\t\t-t\t\tshow table names\n\t\t");
    u_fprintf(u_stdout,"-V\t\tshow vectors\n\n\t-L\t\tshow selected locale\n\n\t");
    u_fprintf(u_stdout,"Help and version:\n\t\t-h\t\t");
    u_fprintf(u_stdout,"show help\n\t\t-v\t\tshow version.\n\n");
    exit(EXIT_SUCCESS);
}

static void usage(void){
    u_fprintf(u_stdout,"%S: [OPTIONS] -d [DIRECTORY TO SEARCH] -n [FILE NAME]\n", 
            program_name);
}

static void print_version(void){
    u_fprintf(u_stdout,"%S: Version 1.0, 3-2-2014 - copyright (C) Oly Project\n", 
            program_name);
    u_fprintf(u_stdout,"\tLicensed according to the MIT license, which is also the ICU license.\n");
    u_fprintf(u_stdout,"\tThis program is provided in the hopes that it will be useful, but\n");
    u_fprintf(u_stdout,"\tWITHOUT ANY WARRANTY, to the extent permitted by law.\n\n");
    u_fprintf(u_stdout,"\tVisit %s for precise details on authorship.\n", PACKAGE_URL);
}

