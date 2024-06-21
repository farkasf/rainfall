#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	language = 0;

void	greetuser(char *name)
{
	char	greet[64];

	if (language == 1)
		strcpy(greet, "Hyvää päivää ");
	else if (language == 2)
		strcpy(greet, "Goedemiddag! ");
	else
		strcpy(greet, "Hello ");
	strcat(greet, name);
	puts(greet);
}

int	main(int ac, char **av)
{
	char	buff[76];
	char	*lang;

	if (ac != 3)
		return (1);
	strncpy(buff, av[1], 40);
	strncpy(buff + 40, av[2], 32);
	lang = getenv("LANG");
	if (lang)
	{
		if (memcmp(lang, "fi", 2) == 0)
			language = 1;
		else if (memcmp(lang, "nl", 2) == 0)
			language = 2;
	}
	greetuser(buff);
	return (0);
}
