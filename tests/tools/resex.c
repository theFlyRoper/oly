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
#include "oly/oly_dev.h"

static void print_help(void);
static void usage(void);
static void print_version(void);
static void close_main(void);

const char *program_name = "resex";
/* MAIN */
int
main( int argc, char **argv ){
    int32_t         len = 0;
    ochar           line[BUFSIZ];     /* main buffer */
    char            *locale = NULL, *locdir=LOCALEDIR, *opt_error = NULL,
                    *filename = OLY_RESOURCE, c, *find_me = NULL;
    res_disp_flag   flag = {1,0,0,0,0,0,0,0};
    int             i=0;
    UErrorCode      u_status  = U_ZERO_ERROR;
    UResourceBundle *resbund;
    atexit (close_main);
    /* a = arrays, A = Aliases, b = binaries, d = search dir, e = everything,
     * f = find this, h = help, i = integers, l = locales, n = filename, 
     * s = strings, t = tables, v = version, V = vectors
     */
    while ((c = getopt(argc, argv, "aAbd:f:hil:n:stvV")) != -1) 
    {
        switch (c) 
        {
        case ('h'):
            print_help();
            exit(EXIT_SUCCESS);
        case ('v'):
            print_version();
            exit(EXIT_SUCCESS);
        case ('l'):
            locale = optarg;
            break;
        case ('d'):
            locdir = optarg;
            break;
        case ('n'):
            filename = optarg;
            break;
        case ('f'):
            find_me = optarg;
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
        case ('t'):
            flag.all = 0;
            flag.tables = 1;
            break;
        case ('V'):
            flag.all = 0;
            flag.vectors = 1;
            break;
        default:
            printf("Unrecognized option: %s\n", optarg);
            usage();
            exit(EXIT_FAILURE);
            break;
        }
    }

  /* u_setDataDirectory tells ICU where to look for custom app data.  It is not needed
   * for the internal app data for ICU, which lives in a shared library. */
  u_setDataDirectory(locdir);

  if (locale == NULL) {
    locale = oget_user_locale(); 
  }
    resbund = ures_open(filename, locale, &u_status); 
    if (U_FAILURE(u_status)) {
        printf("Could not open resource %s\n", filename);
    }
    list_bundle_resources(resbund, &flag, 0);
  
  if (U_FAILURE(u_status)) {
    exit(EXIT_FAILURE);
  } else {
    exit(EXIT_SUCCESS);
  }
}

static void 
close_main (void) 
{
    if (fclose (stdout) != EXIT_SUCCESS) {
        perror ("resex: write error");
        exit (EXIT_FAILURE);
    }
    if (fclose (stdin) != EXIT_SUCCESS) {
        perror ("resex: read error on close");
        exit (EXIT_FAILURE);
    }
}

static void 
print_help(void)
{
    usage();
    printf("%s: Tool to pry open icu .res / .dat files and browse them.\n", 
            program_name);
    printf("\tCommands:\n\t\t-d [SEARCH DIRECTORY]\n\t\t-f [THING TO FIND]\n");
    printf("\t\t-l [LOCALE TO SEARCH]\n\t\t-n [FILE NAME]\n\n\tFlags:\n\t\t");
    printf("-a\t\tshow names of arrays.\n\t\t-A\t\tshow names of Aliases.\n\t\t");
    printf("-b\t\tbinaries\n\t\t");
    printf("-i\t\tshow integer values.\n\t\t");
    printf("-s\t\tshow string values\n\t\t-t\t\tshow table names.\n\t\t");
    printf("-V\t\tshow vectors\n\n\tHelp and version:\n\t\t-h\t\t");
    printf("show help\n\t\t-v\t\tshow version.\n\n");
    exit(EXIT_SUCCESS);
}

static void usage(void){
    printf("%s: [OPTIONS] -d [DIRECTORY TO SEARCH] -n [FILE NAME]\n", program_name);
}

static void print_version(void){
    printf("%s: Version 1.0, 3-2-2014 - copyright (C) Oly Project\n", program_name);
    printf("\tLicensed according to the MIT license, which is also the ICU license.\n");
    printf("\tThis program is provided in the hopes that it will be useful, but\n");
    printf("\tWITHOUT ANY WARRANTY, to the extent permitted by law.\n\n");
    printf("\tVisit %s for precise details on authorship.\n", PACKAGE_URL);
}

