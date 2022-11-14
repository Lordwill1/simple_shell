#include "main.h"
/**
 * prompt - call prompt from another function (prompt)
 **/
void prompt(void)
{
	for (;;)
	{
		char *text = NULL, **environ;
		pid_t child_pid;
		int status, byte_read;
		size_t bufsize = 0;

		place("$ ");
		byte_read = getline(&text, &bufsize, stdin);
		if (byte_read == -1)
			exit(98);
		if (compareExit(text, "exit") == 0)
			exit(0);
		if (compareEnv(text, "env") == 0)
		{
			while (*environ != NULL)
			{
				if (!(_strcmpdir(*environ, "USER")) ||
						!(_strcmpdir(*environ, "LANGUAGE")) ||
						!(_strcmpdir(*environ, "SESSION")) ||
						!(_strcmpdir(*environ, "COMPIZ_CONFIG_PROFILE")) ||
						!(_strcmpdir(*environ, "SHLV")) ||
						!(_strcmpdir(*environ, "HOME")) ||
						!(_strcmpdir(*environ, "C_IS")) ||
						!(_strcmpdir(*environ, "DESKTOP_SESSION")) ||
						!(_strcmpdir(*environ, "LOGNAME")) ||
						!(_strcmpdir(*environ, "TERM")) ||
						!(_strcmpdir(*environ, "PATH")))
				{
					place(*environ), place("\n");
				}
				environ++;
			}}
		child_pid = fork();
		if (child_pid < 0)
			perror("Error");
		if (child_pid == 0)
			identify_string(text);
		else
			wait(&status); }}
