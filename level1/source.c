#include <stdio.h>
#include <stdlib.h>

void	run(void)
{
	fwrite("Good... Wait what?\n",1, 19, 1);
	system("/bin/sh");
}

int	main(void)
{
	char	buff[80];

	gets(buff);
	return (0);
}
