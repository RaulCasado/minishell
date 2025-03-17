/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executer_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 18:47:07 by racasado          #+#    #+#             */
/*   Updated: 2025/03/15 21:26:34 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_parent_process(int prev_pipe_in, int pipe_fd[2], t_command **cmd)
{
	if (prev_pipe_in != 0)
		close(prev_pipe_in);
	close(pipe_fd[1]);
	*cmd = (*cmd)->next;
	return (pipe_fd[0]);
}

int	setup_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		return (1);
	}
	return (0);
}

int	count_commands(t_command *cmd)
{
	int			num_commands;
	t_command	*tmp;

	num_commands = 0;
	tmp = cmd;
	while (tmp)
	{
		num_commands++;
		tmp = tmp->next;
	}
	return (num_commands);
}

void	cleanup_fds(int fd1, int fd2)
{
	if (fd1 != 0)
		close(fd1);
	if (fd2 != 0)
		close(fd2);
}

void	wait_for_children(t_minishell *minishell)
{
	pid_t	pid;
	int		status;

	pid = waitpid(-1, &status, 0);
	while (pid > 0)
		pid = waitpid(-1, &status, 0);
	if (WIFEXITED(status))
	{
		minishell->exit_code = WEXITSTATUS(status);
		// printf("Exit status of the child was %d\n", minishell->exit_code);
	}
}
