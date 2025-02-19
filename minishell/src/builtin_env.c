
#include "minishell.h"

char	builtin_env(t_command *command, char **envp)
{
	char	fd;

	if (command->args[1]) // There's only 1 arg in env
		return (1);
	if (command->pipe_out == 0)
		fd = 1;
	/* else
		fd = xd; */
	write_strs(envp, 1, 1, fd);
	return (0);
}
