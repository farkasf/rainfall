#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char	c[80];

void	m(void)
{
	int	d_time;

	d_time = (int)time((time_t *)0);
	printf("%s - %d\n", c, d_time);
}

int	main(int ac, char **av)
{
	int	*a;
	int	*b;

	a = malloc(sizeof(int));
	b = malloc(sizeof(int));
	a[0] = 1;
	b[0] = 2;
	a[1] = malloc(sizeof(int));
	b[1] = malloc(sizeof(int));

	strcpy((char *)a[1], av[1]);
	strcpy((char *)b[1], av[2]);
	fgets(c, 68, fopen("/home/user/level8/.pass", "r"));
	puts("~~");
	return (0);
}
