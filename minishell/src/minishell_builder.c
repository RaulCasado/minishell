
#include "minishell.h"

t_minishell	*minishell_builder(char **envp)
{
	t_minishell	*minishell;
	char		**new_envp;

	minishell = malloc(sizeof(struct s_minishell));
	if (!minishell)
		return(NULL);
	minishell->commands = NULL;
	minishell->tokens = NULL;
	minishell->exit_code = 0;
	new_envp = copy_envp(envp);
	if (!new_envp)
	{
		free(minishell);
		return (NULL);
	}
	minishell->envp = new_envp;
	return (minishell);
}
