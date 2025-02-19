
#include "minishell.h"

char	builtin_pwd(t_command *command)
{
	char	*arg;
	char	fd;

	if (command->args[1]) // There's only 1 arg in pwd
		return (1);
	arg = get_env(PWD);
	/* if (!arg)
		error xd */
	if (command->pipe_out == 0)
		fd = 1;
	/* else
		fd = xd; */
	write_str(arg, fd);
	return (0);
}
