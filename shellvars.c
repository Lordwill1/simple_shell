#include "shell.h"
#include "shellvars.h"
/**
 * initsvars - initialize vars
 * @ac: arguemnts intiger
 * @av: string arguments
 * Return: int
 */
int initsvars(int ac, char **av)
{
	ShellVar **specialroot = getspecial();
	ShellVar *special;
	ShellVar *ptr;
	int i = 0;
	char nums[2] = {0, 0};

	/* 0-9, #, $, ?, dash, underscore */
	*specialroot = malloc(sizeof(ShellVar) * 15);
	if (*specialroot == NULL)
		return (-1);
	special = *specialroot;
#ifdef DEBUGMODE
	printf("special:%p:*getspecial():%p:\n", special, *(getspecial()));
#endif
	special->val = _strdup("0");
	special->name = _strdup("?");
	ptr = special + 1;
	special->next = ptr;
	while (av[i] != NULL)
	{
#ifdef DEBUGMODE
		printf("av[%d]=%s\n", i, av[i]);
#endif
		nums[0] = i + '0';
		ptr->val = _strdup(av[i]);
		ptr->name = _strdup(nums);
		ptr->next = ptr + 1;
		ptr = ptr->next;
		i++;
	}
	while (i < 10)
	{
		nums[0] = i + '0';
		ptr->val = _strdup("0");
		ptr->name = _strdup(nums);
		ptr->next = ptr + 1;
		ptr = ptr->next;
		i++;
	}
	ptr->name = _strdup("$");
	ptr->val = _strdup("0");
	ptr->next = ptr + 1;
	ptr = ptr->next;
	ptr->name = _strdup("#");
	ptr->val = itos(ac);
	ptr->next = NULL;
	return (0);
}
/**
 * getsvar - gets shell variable
 * @name: name of shell var
 *
 * Return: original argument if not found
 */
char *getsvar(char *name)
{
	ShellVar *special = *(getspecial()), *vars = *(getvars());
	ShellVar *ptr = special;

	while (ptr != NULL && _strcmp(ptr->name, name))
	{
#ifdef DEBUGMODE
		printf("Checked .%s. against .%s.\n", name, ptr->name);
#endif
		ptr = ptr->next;
	}
	if (ptr != NULL)
	{
#ifdef DEBUGMODE
		printf("Returning special var %s:%s\n", ptr->name, ptr->val);
#endif
		return (_strdup(ptr->val));
	}
	ptr = vars;
	while (ptr != NULL && _strcmp(ptr->name, name))
	{
#ifdef DEBUGMODE
		printf("Checked .%s. against .%s.\n", name, ptr->name);
#endif
		ptr = ptr->next;
	}
	if (ptr == NULL)
	{
#ifdef DEBUGMODE
		printf("Var not found %s\n", name);
#endif
		return (name);
	}
#ifdef DEBUGMODE
	printf("Returning var %s\n", ptr->val);
#endif
	return (_strdup(ptr->val));
}
/**
 * setsvar - sets shell var
 * @name: name of var
 * @val: value of var
 * Return: int
 */
int setsvar(char *name, char *val)
{
	ShellVar **vars = getvars();
	ShellVar *special = *(getspecial());
	ShellVar *ptr = special, *new;

#ifdef DEBUGMODE
	printf("in setsvar, special address: %p\n", special);
	printf("Got %s %s\n", name, val);
#endif
	while (_strcmp(ptr->name, name) && ptr->next != NULL)
	{
		ptr = ptr->next;
	}
	if (!_strcmp(ptr->name, name))
	{
#ifdef DEBUGMODE
		printf("Setting special %s to %s\n", ptr->name, val);
		printf("ptr -> val %p\n", ptr->val);
#endif
		free(ptr->val);
		ptr->val = _strdup(val);
		return (0);
	}
	ptr = *vars;
#ifdef DEBUGMODE
	printf("vars address %p\n", *vars);
#endif
	if (ptr == NULL)
	{
#ifdef DEBUGMODE
		printf("Setting new list %s to %s\n", name, val);
#endif
		new = malloc(sizeof(ShellVar));
		if (new == NULL)
			return (-1);
		new->name = _strdup(name);
		new->val = _strdup(val);
		new->next = NULL;
		*vars = new;
		return (0);
	}
	while (_strcmp(ptr->name, name) && ptr->next != NULL)
		ptr = ptr->next;
	if (ptr != NULL && !_strcmp(ptr->name, name))
	{
#ifdef DEBUGMODE
		printf("Setting %s to %s\n", ptr->name, val);
#endif
		free(ptr->val);
		ptr->val = _strdup(val);
	}
	else
	{
#ifdef DEBUGMODE
		printf("Setting new %s to %s\n", name, val);
#endif
		new = malloc(sizeof(ShellVar));
		if (new == NULL)
			return (-1);
		new->name = _strdup(name);
		new->val = _strdup(val);
		new->next = NULL;
		ptr->next = new;
	}
	return (0);
}
/**
 * unsetsvar - unset shell var
 * @name: name of var to unset
 * Return: int
 */
int unsetsvar(char *name)
{
	ShellVar *vars = *getvars();
	ShellVar *ptr = vars, *next;

#ifdef DEBUGMODE
	printf("In unsetsvar:vars:%p:name:%s\n", vars, name);
#endif
	if (vars == NULL)
		return (0);
#ifdef DEBUGMODE
	printf("ptr->name:%s\n", ptr->name);
#endif
	if (!_strcmp(ptr->name, name))
	{
#ifdef DEBUGMODE
		printf("First node match\n");
#endif
		*vars = *vars->next;
		free(ptr->name);
		free(ptr->val);
		free(ptr);
		return (0);
	}
	while (ptr->next != NULL && _strcmp(ptr->next->name, name))
		ptr = ptr->next;
	if (!_strcmp(ptr->next->name, name))
	{
		next = ptr->next->next;
		free(ptr->next->name);
		free(ptr->next->val);
		free(ptr->next);
		ptr->next = next;
	}
	return (0);
}
