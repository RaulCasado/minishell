/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 21:03:25 by racasado          #+#    #+#             */
/*   Updated: 2025/04/07 21:03:25 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*try_path(char *dir, char *cmd)
{
	char	*path;
	char	*full_path;

	path = ft_strjoin(dir, "/");
	if (!path)
		return (NULL);
	full_path = ft_strjoin(path, cmd);
	free(path);
	if (!full_path)
		return (NULL);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

static char	*search_in_path(char *cmd, char **paths)
{
	int		i;
	char	*result;

	i = 0;
	while (paths[i])
	{
		result = try_path(paths[i], cmd);
		if (result)
			return (result);
		i++;
	}
	return (NULL);
}

char	*find_command_path(char *cmd, char **envp)
{
	char	*direct_path;
	char	**paths;
	char	*result;

	direct_path = handle_direct_path(cmd);
	if (direct_path)
		return (direct_path);
	paths = get_path_directories(envp);
	if (!paths)
		return (NULL);
	result = search_in_path(cmd, paths);
	ft_free_split(paths);
	return (result);
}

static void	handle_exec_errors(t_minishell *ms, char *path, char *cmd_name)
{
	struct stat	st;

	if (errno == ENOEXEC && (cmd_name[0] == '.' || cmd_name[0] == '/'))
	{
		free(path);
		free_minishell(ms);
		exit(0);
	}
	else if (errno == ENOEXEC)
	{
		ft_putstr_fd(cmd_name, 2);
		ft_putendl_fd(": command not found", 2);
		free(path);
		free_minishell(ms);
		exit(2);
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd(cmd_name, 2);
		ft_putendl_fd(": Is a directory", 2);
		free(path);
		free_minishell(ms);
		exit(126);
	}
	ft_putstr_fd(cmd_name, 2);
	ft_putendl_fd(": Permission denied", 2);
	free(path);
	free_minishell(ms);
	exit(126);
}

int	command_process(t_minishell *ms, t_command *cmd)
{
	char	*path;

	if (!cmd->args[0] || !cmd->args[0][0])
	{
		if (cmd->args[1])
			cmd->args++;
		else
		{
			free_minishell(ms);
			exit(0);
		}
	}
	path = find_command_path(cmd->args[0], ms->envp);
	if (!path)
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": command not found", 2);
		free_minishell(ms);
		exit(127);
	}
	execve(path, cmd->args, ms->envp);
	if (errno == EACCES || errno == EISDIR)
		handle_exec_errors(ms, path, cmd->args[0]);
	perror("minishell");
	free(path);
	free_minishell(ms);
	exit(127);
}
