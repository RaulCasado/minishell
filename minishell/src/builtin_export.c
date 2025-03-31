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

static char	*remove_quotes(char *str)
{
	size_t	len;
	char	*new_arg;
	char	*equal_sign;
	char	*value;

	if (!str)
		return (NULL);
	equal_sign = ft_strchr(str, '=');
	if (!equal_sign || !(equal_sign + 1))
		return (str);
	value = equal_sign + 1;
	if (value[0] == DOUBLE_MARK && value[ft_strlen(value) - 1] == DOUBLE_MARK)
	{
		len = ft_strlen(value) - 2;
		new_arg = malloc(sizeof(char) * (equal_sign - str + 1 + len + 1));
		if (!new_arg)
			return (str);
		ft_memcpy(new_arg, str, equal_sign - str + 1);
		ft_memcpy(new_arg + (equal_sign - str + 1), value + 1, len);
		new_arg[equal_sign - str + 1 + len] = '\0';
		return (new_arg);
	}
	return (str);
}

static void	process_export_arg(t_minishell *minishell, char *arg,
	int *exit_code)
{
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("Minishell: export: not a valid identifier", STDERR_FILENO); // It was different
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
	int		exit_code;
	int		i;
	char	*arg;

	exit_code = 0;
	if (!command->args[1])
	{
		print_sorted_env(minishell->envp);
		return (0);
	}
	i = 1;
	while (command->args[i])
	{
		arg = remove_quotes(command->args[i]);
		if (arg != command->args[i])
		{
			free(command->args[i]);
			command->args[i] = arg;
		}
		process_export_arg(minishell, arg, &exit_code);
		i++;
	}
	return (exit_code);
}
