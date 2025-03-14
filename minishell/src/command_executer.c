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

static int	setup_child_process(t_minishell *minishell, t_command *cmd,
		int prev_pipe_in, int pipe_fd[2])
{
	if (prev_pipe_in != 0)
		dup2(prev_pipe_in, STDIN_FILENO);
	if (cmd->next)
		dup2(pipe_fd[1], STDOUT_FILENO);
	if (prev_pipe_in != 0)
		close(prev_pipe_in);
	close(pipe_fd[0]);
	if (cmd->next)
		close(pipe_fd[1]);
	handle_redirections(cmd);
	execute_command(minishell, cmd);
	exit(EXIT_SUCCESS);
	return (0);
}

static int	create_process(t_minishell *minishell, t_command **cmd,
		int *prev_pipe_in, int pipe_fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	else if (pid == 0)
		setup_child_process(minishell, *cmd, *prev_pipe_in, pipe_fd);
	if ((*cmd)->next)
		*prev_pipe_in = handle_parent_process(*prev_pipe_in, pipe_fd, cmd);
	else
	{
		cleanup_fds(*prev_pipe_in, 0);
		*prev_pipe_in = 0;
		*cmd = (*cmd)->next;
	}
	return (0);
}

static int	execute_pipeline(t_minishell *minishell, t_command *cmd)
{
	int		pipe_fd[2];
	int		prev_pipe_in;

	prev_pipe_in = 0;
	while (cmd)
	{
		if (cmd->next && setup_pipe(pipe_fd))
			return (1);
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
	if (num_commands == 1 && is_builtin(cmd->args[0]))
	{
		execute_single_builtin(minishell, cmd);
		return (0);
	}
	return (execute_pipeline(minishell, cmd));
}
