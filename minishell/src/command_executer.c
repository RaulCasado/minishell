
#include "minishell.h"

char	command_executer(t_command *commands, char **envp)
{
	t_command	*cmd;
	size_t		cmd_len;

	cmd = commands;
	while (cmd)
	{
		cmd_len = ft_strlen(cmd->args[0]);

		if (cmd_len == 3
			&& !ft_strncmp(cmd->args[0], "pwd", 3))
			builtin_pwd(cmd);
		else if (cmd_len == 3
			&& !ft_strncmp(cmd->args[0], "env", 3))
			builtin_env(cmd, envp);
		else if (cmd_len == 4
			&& !ft_strncmp(cmd->args[0], "echo", 4))
			builtin_echo(cmd);

		cmd = cmd->next;
	}
	return (0);
}
