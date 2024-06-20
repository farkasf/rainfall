#include <stdio.h>
#include <stdlib.h>

int main()
{
	char *env_var = getenv("SCODE");
	if (env_var == 0)
		printf("SCODE not set\n");
	else
		printf("SCODE set at %p\n", (void*)env_var);
	return 0;
}
