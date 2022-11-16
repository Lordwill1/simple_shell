#include "shell.h"
/**
 * strtok - tokenizes a string
 * @str: string to tokenize
 *
 * @delim: delimiters used to create tokens
 *
 * Return: token
 */
char *strtok(char *str, char *delim)
{
	static char *saved_string;
	int i;
	int j;
	char *tmp_str;
	char *tmp_delim;

	/*if NULL passed in str becomes where saved string left off*/
	if (str == 0)
		str = saved_string;
	if (str == 0)
		return (0);

	tmp_str = str;
	tmp_delim = delim;
	/*skip initial delimiters*/
	i = 0;
	while (tmp_str[i] != 0)
	{
		j = 0;
		while (delim[j] != 0)
		{
			if (tmp_str[i] == tmp_delim[j])
				break;
			j++;
		}
		if (tmp_delim[j] == 0)
			break;
		i++;
	}
	str = str + i;
	if (*str == 0)
	{
		saved_string = str;
		return (0);
	}
	/*start new token*/
	tmp_str = tmp_str + i;

	i = 0;
	while (tmp_str[i] != 0)
	{
		j = 0;
		while (tmp_delim[j] != 0)
		{
			if (tmp_str[i] == tmp_delim[j])
				break;
			j++;
		}
		if (tmp_delim[j] != 0)
			break;
		i++;
	}
	saved_string = tmp_str;
	if (tmp_str[i] != 0)
	{
		/*saves string for next call*/
		saved_string = (saved_string + i + 1);
		tmp_str[i] = '\0';
	}
	else
	{
		saved_string = '\0'; /*if end of input string.*/
	}
	return (tmp_str);
}
/**
 * strtokqe - string token with quotes and escapes
 * @str: string
 * @delim: delimiters
 * @escflags: escape flags
 * flags are bitwise.
 * 1 = use \ to escape delims
 * 2 = single quote skips
 * 4 = double quote skips
 *
 * Return: string
 */
char *strtokqe(char *str, char *delim, int escflags)
{
	static char *saved_string;
	int i;
	int j;

	/*if NULL passed in str becomes where saved string left off*/
	if (str == 0)
		str = saved_string;
	if (str == 0)
		return (0);

	/*skip initial delimiters*/
	i = 0;
	while (str[i] != 0)
	{
		j = 0;
		while (delim[j] != 0)
		{
			if (str[i] == delim[j])
				break;
			j++;
		}
		if (delim[j] == 0)
			break;
		i++;
	}
	str = str + i;
	if (*str == 0)
	{
		saved_string = str;
		return (0);
	}

	/*start new token*/
	i = 0;
	while (str[i] != 0)
	{
		if (str[i] == '\\' && escflags & 1)
		{
			if (str[i + 1] != 0)
				i++;
			i++;
			continue;
		}
		if (str[i] == '\'' && escflags & 2)
		{
			i++;
			while (str[i] != '\'')
			{
				if (str[i] == '\\' && escflags & 1)
				{
					if (str[i + 1] != 0)
						i++;
					i++;
					continue;
				}
				i++;
			}
		}
		if (str[i] == '"' && escflags & 4)
		{
			i++;
			while (str[i] != '"')
			{
				if (str[i] == '\\' && escflags & 1)
				{
					if (str[i + 1] != 0)
						i++;
					i++;
					continue;
				}
				i++;
			}
		}
		j = 0;
		while (delim[j] != 0)
		{
			if (str[i] == delim[j])
				break;
			j++;
		}
		if (delim[j] != 0)
			break;
		i++;
	}
	saved_string = str;
	if (str[i] != 0)
	{
		/*saves string for next call*/
		saved_string = (saved_string + i + 1);
		str[i] = '\0';
	}
	else
	{
		saved_string = '\0'; /*if end of input string.*/
	}
	return (str);
}
