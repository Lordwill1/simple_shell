#ifndef SHELLH
#define SHELLH

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "history.h"
#include "shellvars.h"
/*#include <string.h>*/

/* from in.c */
int shintmode(void);

/* from _printenv.c */
int _printenv(void);

/* from cmdcall.c */
int builtincall(char *av[]);
int cmdcall(char *av[], char *path);

/* from parser.c */
int parseargs(char **buf);

/* from errhandl.c */
int errhandl(int status);

/* from string.c */
size_t _strlen(char *str);
char *_strcpy(char *dest, char *src);
int _strcmp(char *, char *);
char *_strdup(char *str);
char *_strcat(char *a, char *b);

/* from _getenv.c and getenviron.c */
char ***getenviron(void);
int setallenv(char **environ, char *add);
char *_getenv(char *avzero);
int _setenv(char *name, char *val);
int _unsetenv(char *name);
char **getallenv(void);


/* from utility.c */
char *itos(int digits);
char *_strchr(char *s, char c);
int fprintstrs(int fd, char *str, ...);
int printerr(char *);
int linecount(int);

/* from cd.c */
int _cd(char *av[]);

/* from alias.c */
int aliascmd(char **av);
char *getalias(char *name);
int unsetalias(char *name);

/* from shellvars.c */
int initsvars(int ac, char **av);
char *getsvar(char *name);
int setsvar(char *name, char *val);
int unsetsvar(char *name);
ShellVar **getspecial(void);
ShellVar **getvars(void);

/* from _realloc.c */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);

/* from _strtok.c */
char *strtok(char *str, char *delim);

/* from _getline.c */
int _getline(char **lineptr, int fd);

char *strtokqe(char *str, char *delim, int escflags);

/*from history.c*/
int sethist(char *cmd);
int print_hist(void);
int exit_hist(void);


/* from _printenv.c */
int _printenv(void);
int _putchar(char c);



/*from help.c*/
int help(char *cmd);

/* from exitcleanup.c */
void exitcleanup(char **av);

/* from _atoi*/
int _atoi(char *s);

char *_getpid(void);


#endif
