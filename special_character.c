#include "main.h"

/**
 * lookforslash - identifies if first char is a slash.
 * @cmd: first string
 * Return: 1 if yes 0 if no.
 */
int lookforslash(char *cmd)
{
	int cont = 0;

	while (cmd[cont])
	{
		if (cmd[0] == '/')
		{
			printf("%c\n", cmd[0]);
			return (1);
		}

		cont++;
	}
	return (0);
}

/**
 * compareExit - Checks if the user is typed exit
 * @s1: first string
 * @s2: exit string
 * Return: 1 if yes 0 if no.
 */
int compareExit(char *s1, char *s2)
{
	int i = 0;

	for (; (*s2 != '\0' && *s1 != '\0') && *s1 == *s2; s1++)
	{
		if (i == 3)
			break;
		i++;
		s2++;
	}

	return (*s1 - *s2);
}

/**
 * compareEnv - checks if the used typed env.
 * @s1: first string
 * @s2: exit string
 * Return: 1 if yes 0 if no.
 */
int compareEnv(char *s1, char *s2)
{
	int i = 0;

	for (; (*s2 != '\0' && *s1 != '\0') && *s1 == *s2; s1++)
	{
		if (i == 2)
			break;
		i++;
		s2++;
	}

	return (*s1 - *s2);
}
/**
 * identify_string - identyfy keyboard input.
 * @parameter: call prompt from another function (prompt)
 * Return: str
 **/
char **identify_string(char *parameter)
{
	char **buf = malloc(1024 * sizeof(char *));
	char *split;
	int i = 0;
	char *delim = " \t\n";


	split = strtok(parameter, delim);

	while (split != NULL)
	{
		buf[i] = split;
		i++;
		split = strtok(NULL, delim);
	}
	execute_proc(buf);
	return (buf);
}
/**
 * controlC - avoid close the shell
 * @sig: keep going shell
 **/
void  controlC(int sig)
{
	(void) sig;
	write(1, "\n$ ", 3);
}
