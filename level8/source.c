#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char	*auth;
char	*service;

int	main(void)
{
	char	buff[128];
	auth = NULL;
	service = NULL;
	do
	{
		printf("%p, %p\n", (void *)auth, (void *)service);
		if (fgets(buff, sizeof(buff), stdin) == NULL)
			break;
		if (strncmp(buff, "auth ", 5) == 0)
		{
			auth = malloc(4);
			*auth = 0;
			if (strlen(buff) + 5 < 32)
				strcpy(auth, buff + 5);
		}
		if (strncmp(buff, "reset", 5) == 0)
			free(auth);
		if (strncmp(buff, "service", 7) == 0)
			service = strdup(buff + 7);
		if (strncmp(buff, "login", 5) == 0)
		{
			if (auth && auth[32] != '\0')
				system("/bin/sh");
			else
				fwrite("Password:\n", 1, 10, stdout);
		}
	} while (1);
}
