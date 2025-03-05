
#include "minishell.h"

char	builtin_env(t_minishell *minishell, t_command *command)
{
	char	fd;

	if (command->args[1])
		return (1);
	
	fd = STDOUT_FILENO;
	write_strs(minishell->envp, 1, 1, fd);
	return (0);
}
