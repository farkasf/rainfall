#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	m(void)
{
	puts("Nope");
}

void	n(void)
{
	system("/bin/cat /home/user/level7/.pass");
}

int	main(int ac, char **av)
{
	char	*buff;
	void	(*func)(void);

	buff = malloc(64);
	func = malloc(4);
	func = (void (*)(void))m;
	strcpy(buff, av[1]);
	(*func)();

	return (0);
}
