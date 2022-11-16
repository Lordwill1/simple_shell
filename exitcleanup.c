#include "shell.h"
#include "alias.h"
#include "shellvars.h"

char ***getenviron();
ShellVar **getspecial();
ShellVar **getvars();
AliasData **getalist();

/**
 * exitcleanup - cleans up various vars before exiting
 * @av: argument list to free (if any)
 * Return: void
 */

void exitcleanup(char **av)
{
	ShellVar *sptr = *(getspecial()), *snext;
	AliasData *aptr = *(getalist()), *anext;
	char **environ = *(getenviron());
	int i = 0;

	if (av != NULL)
		for (i = 0; av[i] != NULL; i++)
			free(av[i]);
	i = 0;
	while (environ[i] != NULL)
		free(environ[i++]);
	free(environ);
	while (sptr != NULL)
	{
		free(sptr->val);
		free(sptr->name);
		sptr = sptr->next;
	}
	free(*(getspecial()));
	sptr = *(getvars());
	while (sptr != NULL)
	{
		free(sptr->val);
		free(sptr->name);
		snext = sptr->next;
		free(sptr);
		sptr = snext;
	}
	while (aptr != NULL)
	{
		free(aptr->val);
		free(aptr->name);
		anext = aptr->next;
		free(aptr);
		aptr = anext;
	}
}
