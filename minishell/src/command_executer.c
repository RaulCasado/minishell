/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:30:34 by racasado          #+#    #+#             */
/*   Updated: 2025/03/14 17:30:34 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_child_process(t_minishell *minishell, t_command *cmd,
	int prev_pipe_in, int pipe_fd[2])
{
	// Only dup2 if prev_pipe_in is valid (not -1)
	if (prev_pipe_in != -1)
	{
		dup2(prev_pipe_in, STDIN_FILENO);
		close(prev_pipe_in);
	}
	if (cmd->next)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	if (pipe_fd[0] >= 0)
		close(pipe_fd[0]);
	if (pipe_fd[1] >= 0)
		close(pipe_fd[1]);
	handle_redirections(cmd);
	execute_command(minishell, cmd);
	exit(EXIT_SUCCESS);
}

static int	create_process(t_minishell *minishell, t_command **cmd,
	int *prev_pipe_in, int pipe_fd[2])
{
	pid_t	pid;
	int		new_pipe;

	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), 1);
	else if (pid == 0)
		setup_child_process(minishell, *cmd, *prev_pipe_in, pipe_fd);
	if ((*cmd)->next)
	{
		new_pipe = handle_parent_process(*prev_pipe_in, pipe_fd, cmd);
		if (new_pipe >= 0)
			*prev_pipe_in = new_pipe;
		else
			*prev_pipe_in = 0;
	}
	else
	{
		*cmd = (*cmd)->next;
		if (*prev_pipe_in > 0)
		{
			cleanup_fds(*prev_pipe_in, 0);
			*prev_pipe_in = 0;
		}
	}
	return (0);
}

static void	execute_single_builtin(t_minishell *minishell, t_command *cmd)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	handle_redirections(cmd);
	execute_command(minishell, cmd);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

static int	execute_pipeline(t_minishell *minishell, t_command *cmd)
{
	int		pipe_fd[2];
	int		prev_pipe_in;

	pipe_fd[0] = -1; // Default: invalid
	pipe_fd[1] = -1;
	// Use -1 for an invalid fd (instead of 0, which is STDIN)
	prev_pipe_in = -1;
	while (cmd)
	{
		if (cmd->next)
		{
			if (setup_pipe(pipe_fd))
				return (1);
		}
		else
		{
			// For the last command, ensure no pipe is used.
			pipe_fd[0] = -1;
			pipe_fd[1] = -1;
		}
		if (create_process(minishell, &cmd, &prev_pipe_in, pipe_fd))
			return (1);
	}
	wait_for_children(minishell);
	return (0);
}

char	command_executer(t_minishell *minishell)
{
	t_command	*cmd;
	int			num_commands;

	cmd = minishell->commands;
	num_commands = count_commands(cmd);
	// Single builtin: execute in parent with proper fd backup/restoration
	if (num_commands == 1 && is_builtin(cmd->args[0]))
	{
		int saved_stdin = dup(STDIN_FILENO);
		int saved_stdout = dup(STDOUT_FILENO);
		handle_redirections(cmd);
		execute_command(minishell, cmd);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (0);
	}
	return (execute_pipeline(minishell, cmd));
}
