/**
 * getenviron - static wrapper copy whole environ
 *
 * Return: environ address
 */

char ***getenviron()
{
	static char **environ;

	return (&environ);
}
