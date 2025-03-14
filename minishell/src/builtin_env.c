/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:23:09 by racasado          #+#    #+#             */
/*   Updated: 2025/03/14 17:23:11 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_minishell *minishell, t_command *command)
{
	int	fd;
	int	write_result;

	if (command->args[1])
	{
		ft_putendl_fd("Minishell: env: too many arguments", STDERR_FILENO);
		return (2);
	}
	fd = STDOUT_FILENO;
	write_result = write_strs(minishell->envp, 1, 1, fd);
	if (write_result == -1)
	{
		perror("Minishell: env");
		return (1);
	}
	return (0);
}
