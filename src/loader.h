/*
 * File loader.h
 *
 * load and initialize everything.
 * used in main.c and the loading functions
 */

#ifndef LOADER_H
#define LOADER_H 1

extern char **environ; 
char *oget_home (struct passwd *pwd);

extern void close_oly (void);
extern void init_io(const char *locale);

extern int cleanenv (void);

#endif /* LOADER_H */
