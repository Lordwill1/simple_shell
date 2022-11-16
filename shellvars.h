#ifndef SHELLVARH
#define SHELLVARH

/**
 * struct ShellVar - shellvar list node
 * @name: name of variable
 * @val: value of variable
 * @next: next variable in the list
 */
typedef struct ShellVar
{
	char *name;
	char *val;
	struct ShellVar *next;
} ShellVar;

#endif
