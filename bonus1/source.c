#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int	main(int ac, char **av)
{
	char	buff[40];
	int		num;

	num = atoi(av[1]);
	if (num > 10)
		return (1);
	else
	{
		memcpy(buff, av[2], num * 4);
		if (num == 1464814662)
			execl("/bin/sh", "sh", 0);
	}
	return (0);
}
