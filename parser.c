#include "shell.h"
/**
 * parsesetsvar - parse set shell vars, returns new buf after var setting
 * @buf: buffer
 * Return: string
 */
char *parsesetsvar(char *buf)
{
	int haseq;
	char *ptr, *name, *val, *bufstart = buf, *newbuf;

	do {
		haseq = 0;
		for (ptr = buf; *ptr; ptr++)
		{
#ifdef DEBUGSVARS
			printf("in loop ptr:%s\n", ptr);
#endif
			if (*ptr == '=')
			{
				name = strtok(buf, " ");
				buf = strtok(NULL, "");
				ptr = buf;
				haseq = 1;
				name = strtok(name, "=");
				val = strtok(NULL, "");
#ifdef DEBUGSVARS
				printf("In parsesetvar: setting var %s to %s\n", name, val);
#endif
				setsvar(name, val);
				if (buf == NULL)
				{
					free(bufstart);
					return (NULL);
				}
				continue;
			}
			if (*ptr == ' ' || *ptr == 0)
			{
				buf = _strdup(buf);
				/* do malloc fail test */
				free(bufstart);
				return (buf);
			}
			if (ptr == NULL)
			{
#ifdef DEBUGSVARS
				printf("No other args found, returning\n");
#endif
				free(bufstart);
				return (NULL);
			}
		}
	} while (haseq && *ptr != 0);
	newbuf = malloc(sizeof(char) * (_strlen(buf) + 1));
	if (newbuf == NULL)
		return (NULL);
	newbuf = _strcpy(newbuf, buf);
	free(bufstart);
	return (newbuf);
}

/**
 * subsvars - substitutes in svars for $names
 * @buf: buffer string that was input
 * Return: processed buffer string
 */
char *subsvars(char **buf)
{
	char *varptr = *buf, *ptr, *name, *val, *valptr, *dest, *dolptr;
	size_t buflen = _strlen(*buf);
	size_t varvlen, varnlen, i;
	int inquotes = 0;

#ifdef DEBUGSVARS
	printf("In subsvars\n");
#endif
	while (*varptr != 0)
	{
#ifdef DEBUGSVARS
		printf("Top of svar loop buf:%s::varptr:%s\n", *buf, varptr);
#endif
		while (*varptr != '$' && *varptr != 0)
		{
/*			printf("inquotes:%d:varptr:%s\n", inquotes, varptr);*/
			if (*varptr == '\\')
			{
				varptr++;
				if (*varptr != 0)
					varptr++;
				continue;
			}
			if (inquotes == 2 && *varptr == '"')
				inquotes = 0;
			if (inquotes == 0 && *varptr == '"')
				inquotes = 2;
			if (*varptr == '\'' && inquotes != 2)
			{
				varptr++;
				while (*varptr != '\'' && *varptr != 0)
					varptr++;
			}
			varptr++;
			if (*varptr == '$' &&
			    (varptr[1] == ' ' || varptr[1] == 0 || varptr[1] == '\n'))
				varptr++;
		}
#ifdef DEBUGSVARS
		printf("At $:%s\n", varptr);
#endif
		dolptr = varptr;
		if (*varptr == 0)
			return (*buf);
		varptr++;
		for (ptr = varptr, varnlen = 0; *ptr != 0 && *ptr != ' '
			     && *ptr != '\n' && *ptr != '\\'; ptr++)
			varnlen++;
#ifdef DEBUGSVARS
		printf("varnlen:%d varptr:%s\n", varnlen, varptr);
#endif
		name = malloc(sizeof(char) * (varnlen + 1));
		if (name == NULL)
			return (NULL);
		for (i = 0; i < varnlen; i++, varptr++)
			name[i] = *varptr;
		name[i] = 0;
#ifdef DEBUGSVARS
		printf("Name got:%s\n::varptr:%s\n", name, varptr);
#endif
		val = _getenv(name);
		if (val == name)
		{
#ifdef DEBUGSVARS
			printf("%s not an env var, checking svars\n", name);
#endif
			val = getsvar(name);
			if (val == name)
				val = _strdup("");
		}
		free(name);
#ifdef DEBUGSVARS
		printf("val got:%s\n", val);
#endif
		varvlen = _strlen(val);
/*
 *need new buffer for substituted var string
 */
		buflen = buflen - varnlen + varvlen + 1;
#ifdef DEBUGSVARS
		printf("malloc size:%d\n", buflen);
#endif
		name = malloc(sizeof(char) * (buflen));
		for (ptr = *buf, dest = name, valptr = val; *ptr != 0; ptr++, dest++)
		{
/*
 * printf("copy to new buf %s::%s\n", ptr, name);
 */
			if (val != NULL && ptr == dolptr)
			{
				while (*valptr != 0)
					*dest++ = *valptr++;
				free(val);
				val = NULL;
				varptr = dest;
				ptr += varnlen + 1;
				if (*ptr == 0)
					break;
			}
			*dest = *ptr;
		}
		*dest = *ptr;
#ifdef DEBUGSVARS
		printf("Resulting buf:%s::varptr:%s\n", name, varptr);
#endif
		free(*buf);
		*buf = name;
	}
	return (*buf);
}

/**
 * cleanarg - cleans escapes and functional quotes
 *
 * @arg: - argument to clean
 *
 * Return: cleaned argument
 */
char *cleanarg(char *arg)
{
	char *newbuf, *ptr, *ptr2;
	size_t len = 0;
	int inquote = 0;

	ptr = arg;
	while (*ptr != 0)
	{
		if (*ptr == '\\' && !inquote)
		{
			ptr++;
			if (*ptr != 0)
			{
				len++;
				ptr++;
			}
			continue;
		}
		if (*ptr == '\\' && inquote == 2)
		{
			ptr++;
			if (*ptr == '$' || *ptr == '#' || *ptr == ';' || *ptr == '\\')
			{
				len++;
				ptr++;
			}
			else
				len++;
			continue;
		}
		if (!inquote && *ptr == '"')
		{
			inquote = 2;
			ptr++;
			continue;
		}
		if (!inquote && *ptr == '\'')
		{
			inquote = 1;
			ptr++;
			continue;
		}
		if ((inquote == 1 && *ptr == '\'') || (inquote == 2 && *ptr == '"'))
		{
			inquote = 0;
			ptr++;
			continue;
		}
		if (*ptr == 0)
			break;
		ptr++;
		len++;
	}
	newbuf = malloc(sizeof(char) * (len + 1));
	if (newbuf == NULL)
		return (NULL);
	ptr = arg;
	ptr2 = newbuf;
	inquote = 0;
	while (*ptr != 0)
	{
		if (*ptr == '\\' && !inquote)
		{
			ptr++;
			if (*ptr != 0)
				*ptr2++ = *ptr++;
			continue;
		}
		if (*ptr == '\\' && inquote == 2)
		{
			ptr++;
			if (*ptr == '$' || *ptr == '#' || *ptr == ';' || *ptr == '\\')
				*ptr2++ = *ptr++;
			else
				*ptr2++ = '\\';
			continue;
		}
		if (!inquote && *ptr == '"')
		{
			inquote = 2;
			ptr++;
			continue;
		}
		if (!inquote && *ptr == '\'')
		{
			inquote = 1;
			ptr++;
			continue;
		}
		if ((inquote == 1 && *ptr == '\'') || (inquote == 2 && *ptr == '"'))
		{
			inquote = 0;
			ptr++;
			continue;
		}
		if (*ptr != 0)
			*ptr2++ = *ptr++;
	}
	*ptr2 = 0;
#ifdef DEBUGMODE
	printf("clean arg return buf:%s\n", newbuf);
#endif
	free(arg);
	return (newbuf);
}

/**
 * tildeexpand - handle expanding ~ where appropriate
 *
 * @buf: buffer to process
 *
 * Return: processed buffer
 */
char *tildeexpand(char *buf)
{
	char *tildeptr = buf, *endptr, *homepath, *newbuf, *bufptr, *newptr;
	int inquotes = 0;

#ifdef DEBUGMODE
	printf("In tildeexpand:%s\n", tildeptr);
#endif
	while (*tildeptr != 0)
	{
		tildeptr = buf;
		while (*tildeptr != '~' && *tildeptr != 0)
		{
/*			printf("Finding ~:%s\n", tildeptr);*/
			if (*tildeptr == '\\')
			{
				tildeptr++;
				if (*tildeptr != 0)
					tildeptr++;
				continue;
			}
			if (inquotes != 1 && *tildeptr == '"')
			{
				inquotes = 2;
				while (*tildeptr != '"' && *tildeptr != 0)
				{
					if (*tildeptr == '\\')
					{
						tildeptr++;
						inquotes = *tildeptr != 0 && tildeptr++;
						inquotes = 2;
						continue;
					}
					tildeptr++;
				}
			}
			if (*tildeptr == '\'' && inquotes != 2)
			{
				tildeptr++;
				while (*tildeptr != '\'' && *tildeptr != 0)
					tildeptr++;
			}
			tildeptr++;
		}
		if (*tildeptr == 0)
			return (buf);
		endptr = tildeptr;
		while (*endptr != '/' && *endptr != ' ' && *endptr != 0)
			endptr++;
		homepath = _getenv("HOME");
#ifdef DEBUGMODE
		printf("tildeexpand got homepath:%s\n", homepath);
#endif
		if (homepath == NULL)
			return (NULL);
		newbuf = malloc(_strlen(buf) - (size_t) endptr +
				(size_t) tildeptr + _strlen(homepath) + 1);
		if (newbuf == NULL)
		{
			free(homepath);
			return (NULL);
		}
		bufptr = buf;
		newptr = newbuf;
		while (bufptr < tildeptr)
			*newptr++ = *bufptr++;
		bufptr = homepath;
		while (*bufptr)
			*newptr++ = *bufptr++;
		while (*endptr)
			*newptr++ = *endptr++;
		*newptr = 0;
#ifdef DEBUGMODE
		printf("tilde expanded %s\n", newbuf);
#endif
		free(homepath);
		free(buf);
		buf = newbuf;
	}
	return (newbuf);
}
/**
 * parseargs - parse arguments function, frees buf at end
 * @buf: buffer pointer
 * Return: return value of command
 */
int parseargs(char **buf)
{
	char *av[1024], *ptr, *left, *right;
	int ac, ret = 0, newchk;

#ifdef DEBUGMODE
	printf("In parseargs. buf:%s\n", *buf);
#endif
	if (*buf == NULL || **buf == 0)
		return (0);
	ptr = *buf;
	newchk = _strlen(*buf) - 1;
	if (ptr[newchk] == '\n')
		ptr[newchk] = 0;
	if (*buf[0] == 0)
	{
		free(*buf);
		return (0);
	}
#ifdef DEBUGMODE
	printf("Breaking command segments:%p:%s\n", *buf, *buf);
#endif
	left = _strdup(strtokqe(*buf, ";", 7));
	right = _strdup(strtokqe(NULL, "", 7));
	free(*buf);
	*buf = left;
#ifdef DEBUGMODE
	printf("left cmd %s\n", left);
	printf("right cmd %s\n", right);
#endif
	if (right != NULL && *right != 0)
	{
		parseargs(&left);
		return (parseargs(&right));
	}
#ifdef DEBUGMODE
	printf("Performing logic &&:%s\n", *buf);
#endif
	left = strtokqe(*buf, "&", 7);
	right = strtokqe(NULL, "", 7);
	if (right != NULL && *right == '&')
	{
		/* need to check malloc fails here */
		left = _strdup(left);
		right = _strdup(right);
		free(*buf);
		*buf = left;
#ifdef DEBUGMODE
		printf("left cmd %s\n", left);
		printf("right cmd %s\n", right);
#endif
		ret = parseargs(&left);
		*buf = right;
		right++;
		right = _strdup(right);
		free(*buf);
		if (ret == 0)
			return (parseargs(&right));
		*buf = right;
		strtokqe(right, "|", 7);
		right = strtokqe(NULL, "", 7);
		if (right != NULL)
		{
			right++;
			right = _strdup(right);
			free(*buf);
			return (parseargs(&right));
		}
		free(*buf);
		return (ret);
	}
	else if (right != NULL)
	{
		*(right - 1) = '&';
	}
#ifdef DEBUGMODE
	printf("Performing logic ||:%s\n", *buf);
#endif
	left = strtokqe(*buf, "|", 7);
	right = strtokqe(NULL, "", 7);
	if (right != NULL && *right == '|')
	{
		/* need to check for malloc fails here */
		left = _strdup(left);
		right = _strdup(right);
		free(*buf);
		*buf = left;
#ifdef DEBUGMODE
		printf("left cmd %s\n", left);
		printf("right cmd %s\n", right);
#endif
		ret = parseargs(&left);
		*buf = right;
		right++;
		right = _strdup(right);
		free(*buf);
		if (ret != 0)
			return (parseargs(&right));
		free(right);
		return (ret);
	}
	else if (right != NULL)
	{
		*(right - 1) = '|';
	}
#ifdef DEBUGMODE
	printf("Subbing vars %s\n", *buf);
#endif
	*buf = subsvars(buf);
	if (*buf == NULL)
		return (-1);
#ifdef DEBUGMODE
	printf("expanding tildes %s\n", *buf);
#endif
	*buf = tildeexpand(*buf);
	if (*buf == NULL)
		return (-1);
#ifdef DEBUGMODE
	printf("Setting vars %s\n", *buf);
#endif
	*buf = parsesetsvar(*buf);
	if (*buf == NULL)
		return (0);
	ac = 0;
	av[ac++] = _strdup(strtokqe(*buf, "\n ", 7));
#ifdef DEBUGMODE
	printf("Got arg %s\n", av[ac - 1]);
#endif
	av[0] = getalias(av[0]);
#ifdef DEBUGMODE
	printf("Alias:%s\n", av[0]);
#endif
	if (av[0] != NULL)
		av[0] = cleanarg(av[0]);
#ifdef DEBUGMODE
	printf("Clean arg[%d] %s\n", ac - 1, av[ac - 1]);
#endif
	while (av[ac - 1] != NULL)
	{
		av[ac] = _strdup(strtokqe(NULL, "\n ", 7));
#ifdef DEBUGMODE
		printf("Got arg %s\n", av[ac]);
#endif
		if (av[ac] != NULL)
			av[ac] = cleanarg(av[ac]);
#ifdef DEBUGMODE
		printf("Clean arg %s\n", av[ac]);
#endif
		ac++;
	}
#ifdef DEBUGMODE
	printf("After cleaning\n");
#endif
	ac--;
	av[ac] = NULL;
	free(*buf);
	*buf = NULL;
	ret = builtincall(av);
#ifdef DEBUGMODE
	printf("After cmdcall ret:%d\n", ret);
#endif
#ifdef DEBUGMODE
	printf("Free av strings\n");
#endif
	for (ac = 0; av[ac] != NULL; ac++)
		free(av[ac]);
#ifdef DEBUGMODE
	printf("Returning, ret:%d\n", ret);
#endif
	return (ret);
}
