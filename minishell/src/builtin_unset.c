// i dont fucking know what to do with the remove_env_var function
// Its not malloc allocated so we cant free it, right now this works its "unsets" the env var
// but i dont know if its the right way to do it leaks wise

// possible solution is to malloc our envp array and then free it when we remove an env var so we can control the memory
// but i dont know if this is the right way to do it :D (this was the solution i went with)

// unset is a builtin command that removes environment variables from the environment.
// The unset utility shall remove variable assignments from the environment of the current shell execution environment. aka minishell
// The unset utility shall not affect the current shell execution environment.
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
