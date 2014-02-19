/*
 * File loader.h
 *
 * load and initialize everything.
 * used in main.c and the loading functions
 */
#ifndef BREAK_RULES_H
#define BREAK_RULES_H 1

UBreakIterator *get_rules(const char *ruleFileName, UErrorCode status);

#endif /* BREAK_RULES_H */
