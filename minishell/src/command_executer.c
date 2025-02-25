
#include "minishell.h"

char	command_executer(t_minishell *minishell)
{
	t_command	*cmd;
	size_t		cmd_len;

	cmd = minishell->commands;
	while (cmd)
	{
		cmd_len = ft_strlen(cmd->args[0]);

		if (cmd_len == 4 && !ft_strncmp(cmd->args[0], "echo", 4))
			builtin_echo(minishell, cmd);
		else if (cmd_len == 2 && !ft_strncmp(cmd->args[0], "cd", 3))
			builtin_cd(minishell, cmd);
		else if (cmd_len == 3 && !ft_strncmp(cmd->args[0], "pwd", 3))
			builtin_pwd(minishell, cmd);
		else if (cmd_len == 6 && !ft_strncmp(cmd->args[0], "export", 3))
			builtin_export(minishell, cmd);
		else if (cmd_len == 5 && !ft_strncmp(cmd->args[0], "unset", 3))
			builtin_unset(minishell, cmd);
		else if (cmd_len == 3 && !ft_strncmp(cmd->args[0], "env", 3))
			builtin_env(minishell, cmd);
		else if (cmd_len == 4 && !ft_strncmp(cmd->args[0], "exit", 3))
			builtin_exit(minishell, cmd);
		else
			command_process(minishell, cmd);

		cmd = cmd->next;
	}
	return (0);
}
