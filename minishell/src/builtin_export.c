/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: droura-s <droura-s@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:23:31 by racasado          #+#    #+#             */
/*   Updated: 2025/03/19 12:47:42 by droura-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static void	process_export_arg(t_minishell *minishell, char *arg
	, int *exit_code)
{
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("Minishell: export: `", STDERR_FILENO);
		*exit_code = 1;
		return ;
	}
	if (add_or_replace_env_var(minishell, arg) != 0)
	{
		ft_putendl_fd("Minishell: export: memory allocation failed",
			STDERR_FILENO);
		*exit_code = 1;
	}
}

int	builtin_export(t_minishell *minishell, t_command *command)
{
	int	exit_code;
	int	i;

	exit_code = 0;
	if (!command->args[1])
	{
		print_sorted_env(minishell->envp);
		return (0);
	}
	i = 1;
	while (command->args[i])
	{
		/* if (command->args[i][ft_strlen(command->args[i])-1] == '=')
		{
			process_export_arg(minishell, ft_strjoin(command->args[i], command->args[i + 1]), &exit_code);
			i++;
		}
		else */
		process_export_arg(minishell, command->args[i], &exit_code);
		i++;
	}
	return (exit_code);
}
