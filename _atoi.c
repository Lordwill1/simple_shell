#include "shell.h"

/**
 * _atoi - converts string to number
 * @s : pointer int
 * _atoi: converts string to number
 * Return: The result (converted number)
 */

int _atoi(char *s)
{
	int i;
	int check_num;
	unsigned int sum;
	unsigned int x;
	int neg;

	neg = 0;
	check_num = 0;
	sum = 0;
	i = 0;

	/* run a while loop */
	while (s[i] != '\0')
	{
		if ((s[i] > '9' || s[i] < '0') && check_num > 0)
			break;
		if (s[i] == '-')
			neg++;
		if (s[i] >= '0' && s[i] <= '9')
			check_num++;
		i++;
	}
	i = i - 1;
	x = 1;
	while (check_num > 0)
	{
		sum = sum + ((s[i] - '0') * x);
		x = x * 10;
		i--;
		check_num--;
	}
	if (neg % 2 != 0)
		sum = sum * -1;
	return (sum);
}
