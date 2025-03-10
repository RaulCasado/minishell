#include "minishell.h"

static int	find_env_var(char **envp, const char *var)
{
	int		i;
	size_t	len;

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

static int	is_valid_identifier(const char *var)
{
	int	i;

	if (!var || (!ft_isalpha(var[0]) && var[0] != '_'))
		return (0);
	i = 1;
	while (var[i])
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	remove_env_var(t_minishell *minishell, int pos)
{
	int	i;

	free(minishell->envp[pos]);
	i = pos;
	while (minishell->envp[i] != NULL)
	{
		minishell->envp[i] = minishell->envp[i + 1];
		i++;
	}
	minishell->envp[i] = NULL;
}

int	builtin_unset(t_minishell *minishell, t_command *command)
{
	int	i;
	int	pos;
	int	status;

	status = 0;
	if (!command->args[1])
		return (0);
	i = 0;
	while (command->args[++i] != NULL)
	{
		if (!is_valid_identifier(command->args[i]))
		{
			ft_putstr_fd("Minishell: unset: `", 2);
			ft_putstr_fd(command->args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			status = 1;
		}
		else
		{
			pos = find_env_var(minishell->envp, command->args[i]);
			if (pos != -1)
				remove_env_var(minishell, pos);
		}
	}
	return (status);
}
