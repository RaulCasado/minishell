#include "minishell.h"

static int find_env_var(char **envp, const char *var)
{
	int i;
	size_t len;

	i = 0;
	len = ft_strlen(var);
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], var, len) == 0 && envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static void remove_env_var(t_minishell *minishell, int pos)
{
	int i;

	free(minishell->envp[pos]);
	i = pos;
	while (minishell->envp[i] != NULL)
	{
		minishell->envp[i] = minishell->envp[i + 1];
		i++;
	}
	minishell->envp[i] = NULL;
}

char builtin_unset(t_minishell *minishell, t_command *command)
{
	int i;
	int pos;

	if (!command->args[1])
		return (0);

	i = 1;
	while (command->args[i] != NULL)
	{
		pos = find_env_var(minishell->envp, command->args[i]);
		if (pos != -1)
			remove_env_var(minishell, pos);
		i++;
	}

	return (0);
}
