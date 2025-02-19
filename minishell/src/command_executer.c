
#include "minishell.h"

char	command_executer(t_command *commands)
{
	t_command	*current_cmd;
	size_t		cmd_len;

	current_cmd = commands;
	while (current_cmd)
	{
		cmd_len = ft_strlen(current_cmd->args[0]);

		if (cmd_len == 4
			&& !ft_strncmp(current_cmd->args[0], "echo", 4))
			echo(current_cmd);

		current_cmd = current_cmd->next;
	}
	return (0);
}
