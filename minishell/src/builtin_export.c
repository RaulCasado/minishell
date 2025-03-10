// this function is used to export environment variables
// it can also be used to print the environment variables
// if no arguments are passed
// it will print the environment variables in sorted order
// if arguments are passed, it will add or replace the environment variables

// READ UNSET BUILTIN COMMENTS FIRST PLS
#include "minishell.h"

static int	is_valid_identifier(const char *var)
{
	int	i;

	if (!var || (!ft_isalpha(var[0]) && var[0] != '_'))
		return (0);
	i = 1;
	while (var[i] && var[i] != '=')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_sorted_env(char **envp)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (envp[i])
	{
		j = i + 1;
		while (envp[j])
		{
			if (ft_strncmp(envp[i], envp[j], ft_strlen(envp[i])) > 0)
			{
				tmp = envp[i];
				envp[i] = envp[j];
				envp[j] = tmp;
			}
			j++;
		}
		printf("declare -x %s\n", envp[i]);
		i++;
	}
}

int	builtin_export(t_minishell *minishell, t_command *command)
{
	int	i;
	int	exit_code;
	int	result;

	exit_code = 0;
	if (!command->args[1])
	{
		print_sorted_env(minishell->envp);
		return (0); // Success: printed environment
	}
	i = 1;
	while (command->args[i])
	{
		if (!is_valid_identifier(command->args[i]))
		{
			// Error: invalid identifier (exit code 1)
			ft_putstr_fd("Minishell: export: `", STDERR_FILENO);
			exit_code = 1;
		}
		else
		{
			result = add_or_replace_env_var(minishell, command->args[i]);
			if (result != 0)
			{
				// Error: memory allocation failure (exit code 1)
				ft_putendl_fd("Minishell: export: memory allocation failed",
					STDERR_FILENO);
				exit_code = 1;
			}
		}
		i++;
	}
	return (exit_code);
}
