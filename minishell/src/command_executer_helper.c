/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executer_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 22:23:56 by racasado          #+#    #+#             */
/*   Updated: 2025/04/08 13:45:14 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void better_close(int fd)
{
	if (fd >= 0)
	{
		close(fd);
	}
}

void	ce_setup_child_process(t_minishell *minishell, t_command *cmd,
	int prev_pipe_in, int pipe_fd[2])
{
	if (prev_pipe_in != -1)
	{
		dup2(prev_pipe_in, STDIN_FILENO);
		better_close(prev_pipe_in);
	}
	if (cmd->next)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		better_close(pipe_fd[1]);
	}
	if (pipe_fd[0] >= 0)
		better_close(pipe_fd[0]);
	if (pipe_fd[1] >= 0)
		better_close(pipe_fd[1]);
	if (handle_redirections(cmd))
		exit(1);
	execute_command(minishell, cmd);
	exit(EXIT_SUCCESS);
}

int	ce_init_pipe(t_command *cmd, int pipe_fd[2])
{
	if (cmd->next)
	{
		if (setup_pipe(pipe_fd))
			return (1);
	}
	else
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
	}
	return (0);
}

void	ce_process_parent(t_command **cmd, int *prev_pipe_in, int pipe_fd[2])
{
	int	new_pipe;

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
}

void	ce_wait_for_all_children(t_minishell *minishell, pid_t last_pid)
{
	pid_t	pid;
	int		status;
	int		last_status;

	last_status = 0;
	while (1)
	{
		pid = wait(&status);
		if (pid <= 0)
			break ;
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
	}
	minishell->exit_code = last_status;
}
