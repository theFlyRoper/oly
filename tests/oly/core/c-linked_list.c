/* c-linked_list.c - Oly linked list tests {{{
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

static void close_main(void);

/* MAIN */
int
main( int argc, char **argv ){
    
    atexit (close_main);
  
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
