#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	main(int ac, char **av)
{
	FILE	*file;
	char	buff1[66];
	char	buff2[65];

	file = fopen("/home/user/end/.pass", "r");
	if (!file || ac != 2)
		return (-1);
	fread(buff1, 1, 66, file);
	buff1[atoi(av[1])] = 0;
	fread(buff2, 1, 65, file);
	fclose(file);
	if (strcmp(buff1, av[1]) == 0)
		execl("/bin/sh", "sh", 0);
	else
		puts(buff2);
	return (0);
}
