/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_process_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 18:55:25 by racasado          #+#    #+#             */
/*   Updated: 2025/03/15 21:25:16 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_direct_path(char *cmd)
{
	if (cmd[0] == '/' || cmd[0] == '.')
		return (ft_strdup(cmd));
	return (NULL);
}

char	**get_path_directories(char **envp)
{
	char	*path_env;
	char	**paths;

	path_env = get_env("PATH", envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	return (paths);
}
