#include "minishell.h"

int builtin_pwd(t_minishell *minishell, t_command *command)
{
	char    *pwd;
	char    cwd[CWD_SIZE];
	int     fd;

	if (command->args[1])
	{
		ft_putendl_fd("Minishell: pwd: too many arguments", STDERR_FILENO);
		return (2);
	}
	pwd = get_env("PWD", minishell->envp);
	if (!pwd)
	{
		// Fallback to current directory
		if (!getcwd(cwd, sizeof(cwd)))
		{
			perror("Minishell: pwd");
			// same as cd error
			return (1);  // Generic error
		}
		pwd = cwd;
	}

	fd = STDOUT_FILENO;
	if (write_str(pwd, fd) == -1)
	{
		perror("Minishell: pwd");
		return (1);
	}
	return (0);
}
