/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:23:15 by racasado          #+#    #+#             */
/*   Updated: 2025/03/14 17:23:16 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_fully_number(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

char	builtin_exit(t_minishell *minishell, t_command *cmd)
{
	int	exit_code;

	if (count_args(cmd->args) > 2)
	{
		ft_putendl_fd("Minishell: exit: too many arguments", 2);
		return (1);
	}
	if (cmd->args[1])
	{
		if (!is_fully_number(cmd->args[1]))
		{
			ft_putendl_fd("Minishell: exit: numeric argument required", 2);
			exit_code = 2;
		}
		else
			exit_code = ft_atoi(cmd->args[1]);
	}
	else
		exit_code = minishell->exit_code;
	printf("Saliendo de Minishell...\n");
	free_minishell(minishell);
	exit(exit_code);
}
