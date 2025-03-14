/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:23:01 by racasado          #+#    #+#             */
/*   Updated: 2025/03/14 17:23:01 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	is_flag(char flag, char *arg)
{
	size_t	i;

	if (ft_strlen(arg) >= 2 && arg[0] == '-')
	{
		i = 1;
		while (arg[i] == flag)
		{
			i++;
			if (!arg[i])
				return (1);
		}
	}
	return (0);
}

int	builtin_echo(t_minishell *minishell, t_command *command)
{
	size_t	i;
	char	**args;
	char	newline;
	int		fd;

	i = 1;
	args = command->args;
	newline = 1;
	while (args[i] && is_flag('n', args[i]))
	{
		newline = 0;
		i++;
	}
	fd = STDOUT_FILENO;
	if (write_strs(args + i, newline, 0, fd) == -1)
	{
		perror("Minishell: echo");
		return (1);
	}
	return (0);
}
