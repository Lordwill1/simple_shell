#include "shell.h"
/**
 * _putchar - writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _putchar(char c)
{
	return (write(1, &c, 1));
}
/**
 * _printenv - print environment
 * Return: 0 if success
 */
int _printenv(void)
{
	char **environ = *(getenviron());
	int i;
	int j;

	i = 0;
	while (environ[i])
	{
		j = 0;
		while (environ[i][j] != 0)
		{
			_putchar(environ[i][j]);
			j++;
		}
		_putchar('\n');
		i++;
	}
	return (0);
}
