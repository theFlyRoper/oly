/*
 * File loader.h
 *
 * load and initialize everything.
 * used in main.c, get_rules.c
 */

#ifndef LOADER_H
#define LOADER_H 1

extern UFILE *u_stdout;
extern UFILE *u_stdin;

UBreakIterator  *get_rules(const char *ruleFileName, UErrorCode status);

extern void print_help (void);
extern void print_version (void);

extern void close_oly (void);
extern void init_io(const char *locale);
extern int cleanenv (Oly *oly);

extern Oly_Status init_all (Oly *oly, char *locale);


#endif /* LOADER_H */
