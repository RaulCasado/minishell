/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:29:47 by racasado          #+#    #+#             */
/*   Updated: 2025/03/30 22:01:00 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_minishell *minishell, t_command *command)
{
	char	*pwd;
	char	cwd[CWD_SIZE];
	int		fd;

	(void) command;
	pwd = get_env("PWD", minishell->envp);
	if (!pwd)
	{
		if (!getcwd(cwd, sizeof(cwd)))
		{
			perror("Minishell: pwd");
			return (1);
		}
		pwd = cwd;
	}
	fd = STDOUT_FILENO;
	if (write_str(pwd, fd) == -1)
		return (perror("Minishell: pwd"), 1);
	return (0);
}
