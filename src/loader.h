/*
 * File loader.h
 *
 * load and initialize everything.
 * used in main.c, get_rules.c
 */

#ifndef LOADER_H
#define LOADER_H 1

UBreakIterator *get_rules(const char *ruleFileName, UErrorCode status);

#endif /* LOADER_H */
