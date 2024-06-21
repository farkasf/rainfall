#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	p(char *output, char *separator)
{
	char	buff[4104];

	puts(separator);
	read(0, buff, 4096);
	buff[strchr(buff, '\n') - buff] = '\0';
	strncpy(output, buff, 20);
}

void	pp(char *output)
{
	char	entry1[20];
	char	entry2[20];
	int		size;

	p(entry1, " - ");
	p(entry2, " - ");
	strcpy(output, entry1);
	size = strlen(output);
	output[size] = ' ';
	output[size + 1] = '\0';
	strcat(output, entry2);
}

int	main(void)
{
	char	output[54];

	pp(output);
	puts(output);
	return (0);
}
