/*
* File get_rules.c
*/
#ifdef HAVE_CONFIG_H
#  include "olyconf.h"
#endif

#include <unicode/utypes.h>
#include <unicode/ucnv.h>
#include <unicode/uclean.h>
#include <unicode/udata.h>
#include <unicode/ustring.h>
#include <unicode/ubrk.h>
#include <unicode/ustdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "oly/common.h"
#include "oly/break_rules.h"
#include "oly/state.h"

UBreakIterator* 
get_rules(const char *ruleFileName, UErrorCode status) {
    /*  Read in the rule source file */
    long        result;
    long        ruleFileSize;
    FILE        *file;
    OFILE       *ufile;
    UBreakIterator *return_me;

    file = fopen(ruleFileName, "rb");
    if( file == 0 ) {
        fprintf(stderr, "Could not open file \"%s\"\n", ruleFileName);
        exit(-1);
    }
    fseek(file, 0, SEEK_END);
    ruleFileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *ruleBufferC = (char *) xmalloc (ruleFileSize + 1);
    ruleBufferC[ruleFileSize] = '\0';
    result = (long)fread(ruleBufferC, 1, ruleFileSize, file);
    if (result != ruleFileSize)  {
        fprintf(stderr, "Error reading file \"%s\"\n", ruleFileName);
        exit (-1);
    }
    
    /* Look for a Unicode Signature (BOM) on the rule file */
    int32_t        signatureLength;
    const char *   ruleSourceC = ruleBufferC;
    const char*    encoding = ucnv_detectUnicodeSignature(
                           ruleSourceC, ruleFileSize, &signatureLength, &status);
    /* fprintf(stderr, "DetectUnicodeSig: \"%s\"\n", encoding); */
    if (U_FAILURE(status)) 
    {
        fprintf(stderr, "\nCan not initialize ICU.  status = %s\n",
            u_errorName(status));
        exit(1);
    }
    if(encoding!=NULL )
    {
        ruleSourceC  += signatureLength;
        ruleFileSize -= signatureLength;
    }
    /* fprintf(stderr, "encoding: \"%s\"\n", encoding); */

    /* Open a converter to take the rule file to UTF-16 */
    UConverter* conv;
    conv = ucnv_open(encoding, &status);
    if (U_FAILURE(status)) {
        fprintf(stderr, "ucnv_open: ICU Error \"%s\"\n", u_errorName(status));
        exit(1);
    }

    ufile = u_finit(file, NULL, NULL);
    u_frewind(ufile);
    UChar *ruleSourceU = (UChar *) xmalloc ((ruleFileSize*sizeof(UChar))+1);
    long charsRead = u_file_read(ruleSourceU, ruleFileSize, ufile);
    /* u_fprintf(u_stderr, "Chars read: \"%i\", File size: \"%i\"\n", charsRead, ruleFileSize); */
    ruleSourceU[charsRead] = 0;
    /* u_fprintf(u_stderr, "RulesourceU POST: \"%S\"\n", ruleSourceU); */
    ucnv_close(conv);
    u_fclose(ufile);

    /*  Create the break iterator from the rules */
    /*     This will compile the rules. */
    UParseError parseError;
    parseError.line = 0;
    parseError.offset = 0;
    return_me = ubrk_openRules(ruleSourceU, ruleFileSize, NULL, 0, &parseError, &status);
    if (U_FAILURE(status)) {
        fprintf(stderr, "createRuleBasedBreakIterator: ICU Error \"%s\"  at line %d, column %d\n",
                u_errorName(status), (int)parseError.line, (int)parseError.offset);
        exit(1);
    };

    return return_me;
}

