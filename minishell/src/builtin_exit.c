
#include "minishell.h"

/*
	- Free everything
	- Exit()?
*/
char	builtin_exit(t_minishell *minishell, t_command *command)
{
	free_minishell(minishell);
	exit(0);
	return (0);
}
