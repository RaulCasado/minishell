
#include "minishell.h"

t_minishell	*minishell_builder(char **envp)
{
	t_minishell	*minishell;

	minishell = malloc(sizeof(struct s_minishell));
	if (!minishell)
		return(NULL);
	minishell->commands = NULL;
	minishell->tokens = NULL;
	minishell->envp = envp;
	return (minishell);
}
