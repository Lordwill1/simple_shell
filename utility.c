#include "shell.h"
#include <stdarg.h>
/**
 * linecount - count lines
 *
 * @increment: icnrementer
 *
 * Return: count
 */
int linecount(int increment)
{
	static int count;

	count = count + increment;

	return (count);
}
/**
 * itos - converts integer to string
 *
 * @digits: int
 *
 * Return: a string
 */
char *itos(int digits)
{
	int count, i, neg, absMod, digitTest;
	char *output;

	digitTest = digits;
	count = 0;
	neg = 0;
	if (digits == 0)
	{
		output = malloc(sizeof(char) * 2);
		output[0] = '0';
		output[1] = 0;
		return (output);
	}
	if (digits < 0)
	{
		neg = 1;
		count++;
	}
	while (digitTest != 0)
	{
		digitTest /= 10;
		count++;
	}
	output = malloc(sizeof(char) * count + 1);
	if (output == NULL)
		return (NULL);
	if (neg)
		output[0] = '-';
	digitTest = digits;
	for (i = count - 1; i >= 0 + neg; i--)
	{
		absMod = digitTest % 10;
		output[i] = (absMod < 0 ? -absMod : absMod) + '0';
		digitTest /= 10;
	}
	output[count] = '\0';
	return (output);

}
/**
 * printerr - printerrors
 * @str: string
 * null prints errno error with perror, otherwise print string as error
 * Return: 0
 */

int printerr(char *str)
{
	char *pathname, *numstr;

	pathname = getsvar("0");
	numstr = itos(linecount(0));
	if (str != NULL)
	{
		fprintstrs(2, pathname, ": ", numstr, str, NULL);
	}
	else
	{
		fprintstrs(2, pathname, ": ", numstr, ": ", NULL);
		perror(NULL);
	}
	free(pathname);
	free(numstr);
	return (0);
}

/**
 * fprintstrs - simple string printer, va args should have a NULL last arg
 * @fd: file descriptor
 * @str: string
 * Return: 0
 */

int fprintstrs(int fd, char *str, ...)
{
	va_list list;

	if (str == NULL)
		return (0);
	va_start(list, str);
	write(fd, str, _strlen(str));
	str = va_arg(list, char *);
	while (str != NULL)
	{
		write(fd, str, _strlen(str));
		str = va_arg(list, char *);
	}
	return (0);
}

/**
 * _strchr - locates a character in a string
 * @s: char pointer
 * @c: char
 * _strchr: locates character in a string and returns a pointer
 * to the first occurence of c in the string s
 * Return: address of first occurence of c in s
 */

char *_strchr(char *s, char c)
{
	int i;
	int len;

	i = 0;
	while (s[i] != '\0')
	{
		len++;
		i++;
	}

	i = 0;
	while (i <= len)
	{
		if (s[i] == c)
			return (s + i);
		i++;
	}
	return (0);
}
