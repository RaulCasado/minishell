
#include "minishell.h"

/*
	- Free everything
	- Exit()?
*/
char	builtin_exit(t_minishell *minishell, t_command *command)
{
	printf("Saliendo de Minishell...\n");
	free_minishell(minishell);
	set_signal(0);
	exit(0);
	return (0);
}
