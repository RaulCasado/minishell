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

static int	free_path_minishell(char *path, t_minishell *ms, int exit_code)
{
	free(path);
	free_minishell(ms);
	exit(exit_code);
}

static void	handle_exec_errors(t_minishell *ms, char *path, char *cmd_name)
{
	struct stat	st;

	if (errno == ENOEXEC && (cmd_name[0] == '.' || cmd_name[0] == '/'))
		free_path_minishell(path, ms, 0);
	else if (errno == ENOEXEC)
	{
		ft_putstr_fd(cmd_name, 2);
		ft_putendl_fd(": command not found", 2);
		free_path_minishell(path, ms, 2);
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd(cmd_name, 2);
		ft_putendl_fd(": Is a directory", 2);
		free_path_minishell(path, ms, 126);
	}
	ft_putstr_fd(cmd_name, 2);
	ft_putendl_fd(": Permission denied", 2);
	free_path_minishell(path, ms, 126);
}

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

int	command_process_execve(t_minishell *ms, t_command *cmd, char *path)
{
	execve(path, cmd->args, ms->envp);
	if (errno == EACCES || errno == EISDIR)
		handle_exec_errors(ms, path, cmd->args[0]);
	perror("minishell");
	return (free_path_minishell(path, ms, 127));
}
