#include <unistd.h>
#include <stdlib.h>

int main(int ac, char **av)
{
	int 	input;
	char	*shell;
	char	*arg[2];
	uid_t	user_id;
	gid_t	group_id;

	input = atoi(av[1]);
	if (input == 423)
	{
		shell = strdup("/bin/sh");
		arg[0] = shell;
		arg[1] = NULL;
		group_id = getegid();
		user_id = getuid();
		setresgid(group_id, group_id, group_id);
		setresuid(user_id, user_id, user_id);
		execv(shell, arg);
	}
	else
		fwrite("No !\n", 1, 5, 2);
	return (0);
}
