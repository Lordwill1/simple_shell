#include "shell.h"
#include "alias.h"
/**
 * getalist - static wrapper getalias list
 * Return: address alist
 *
 */
AliasData **getalist()
{
	static AliasData *alist;

	return (&alist);
}
/**
 * getalias - get alias value
 * returns original argument if not found
 * @name: name of alias
 * Return: original argument if not found, otherwise value
 */
char *getalias(char *name)
{
	AliasData *alist = *(getalist());
	AliasData *ptr = alist;

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
		printf("Alias not found %s\n", name);
#endif
		return (name);
	}
#ifdef DEBUGMODE
	printf("Checking alias %s\n", ptr->val);
#endif

	free(name);
	return (getalias(_strdup(ptr->val)));
}
/**
 * setalias - set alias and value
 * @name: name of alias
 * @val: value to set alias to
 * Return: 0 upon success
 */
int setalias(char *name, char *val)
{
	AliasData **alistroot = getalist();
	AliasData *alist = *alistroot;
	AliasData *ptr = alist, *new;

	if (alist == NULL)
	{
		new = malloc(sizeof(AliasData));
		if (new == NULL)
			return (-1);
		new->name = name;
		new->val = val;
		new->next = NULL;
		*alistroot = new;
		return (0);
	}
	while (_strcmp(ptr->name, name) && ptr->next != NULL)
		ptr = ptr->next;
	if (!_strcmp(ptr->name, name))
	{
		free(ptr->val);
		ptr->val = val;
		free(name);
	}
	else
	{
		new = malloc(sizeof(AliasData));
		if (new == NULL)
			return (-1);
		new->name = name;
		new->val = val;
		new->next = NULL;
		ptr->next = new;
	}
	return (0);
}
/**
 * unsetalias - unset alias of @name
 * @name: name of alias value to unset
 * Return: 0 if sucess
 */
int unsetalias(char *name)
{
	AliasData *alist = *(getalist());
	AliasData *ptr = alist, *next;

	if (alist == NULL)
		return (0);
	if (!(_strcmp(ptr->name, name)))
	{
		alist = alist->next;
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
		free(ptr->next);
		ptr->next = next;
	}
	return (0);
}
/**
 * aliascmd - tokenizes arguments for alias command
 * @av: arguments
 * Return: 1
 */
int aliascmd(char *av[])
{
	AliasData *alist = *(getalist());
	AliasData *ptr = alist;
	int i;
	char *name, *val;

#ifdef DEBUGMODE
	printf("av1 %p ptr %p\n", av[1], ptr);
	printf("av1 %s\n", av[1]);
#endif
	if (av[1] == NULL)
	{
		while (ptr != NULL)
		{
			fprintstrs(1, ptr->name, "='", ptr->val, "'\n", NULL);
			ptr = ptr->next;
		}
		return (0);
	}
#ifdef DEBUGMODE
	printf("Not blank args\n");
#endif
	for (i = 1; av[i] != NULL; i++)
	{
#ifdef DEBUGMODE
		printf("Alias arg %s\n", av[i]);
#endif
		name = strtok(av[i], "=");
		val = strtok(NULL, "=");
		if (val != NULL)
		{
#ifdef DEBUGMODE
			printf("Setting alias:%s:to:%s:\n", name, val);
#endif
			name = _strdup(name);
			if (name == NULL)
				return (-1);
			val = _strdup(val);
			if (val == NULL)
			{
				free(name);
				return (-1);
			}
			setalias(name, val);
		}
		else
		{
#ifdef DEBUGMODE
			printf("Printing alias:%s:\n", name);
#endif
			val = _strdup(name);
			val = getalias(val);
#ifdef DEBUGMODE
			printf("Val:%s\n", val);
#endif
			if (!_strcmp(val, name))
			{
				fprintstrs(1, "alias: ", name, " not found\n", NULL);
				free(val);
			}
			else
			{
				fprintstrs(1, name, "='", val, "'\n", NULL);
				free(val);
			}
		}
	}
	return (0);
}
