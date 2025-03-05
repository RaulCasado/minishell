#include "minishell.h"

char builtin_pwd(t_minishell *minishell, t_command *command)
{
	char	*pwd;
	char	fd;

	if (command->args[1]) // pwd no acepta argumentos
		return (1);

	pwd = get_env("PWD", minishell->envp);
	if (!pwd) // Si no encuentra PWD, usa `getcwd()`
	{
		char cwd[1024];
		if (!getcwd(cwd, sizeof(cwd)))
		{
			perror("Minishell: pwd");
			return (1);
		}
		pwd = cwd;
	}

	fd = STDOUT_FILENO; // Usamos el descriptor de salida estándar
	write_str(pwd, fd);

	return (0);
}
