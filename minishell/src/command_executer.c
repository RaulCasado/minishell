/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>     +#+  +:+       +#+      */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:30:34 by racasado          #+#    #+#             */
/*   Updated: 2025/03/14 17:30:34 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_child_process(t_minishell *minishell, t_command *cmd,
	int prev_pipe_in, int pipe_fd[2])
{
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
	if (handle_redirections(cmd))
		exit(1);
	execute_command(minishell, cmd);
	exit(EXIT_SUCCESS);
}

static int	init_pipe(t_command *cmd, int pipe_fd[2])
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

static void	process_parent(t_command **cmd, int *prev_pipe_in, int pipe_fd[2])
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

static int	process_command_in_pipeline(t_minishell *minishell, t_command **cmd,
	int *prev_pipe_in, pid_t *last_pid)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (init_pipe(*cmd, pipe_fd))
		return (1);
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	else if (pid == 0)
		setup_child_process(minishell, *cmd, *prev_pipe_in, pipe_fd);
	else
	{
		*last_pid = pid;
		process_parent(cmd, prev_pipe_in, pipe_fd);
	}
	return (0);
}

void	wait_for_all_children(t_minishell *minishell, pid_t last_pid)
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

static int	handle_single_output_redirection(t_token *token)
{
	int	fd;

	if (!token->next || !token->next->value)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token", 2);
		return (-1);
	}
	if (token->type == TOKEN_REDIR_OUT)
		fd = open(token->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(token->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(token->next->value, 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	check_output_redirections(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIR_OUT
			||current->type == TOKEN_REDIR_APPEND)
		{
			if (handle_single_output_redirection(current) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}

static int	check_input_redirections(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIR_IN)
		{
			if (!current->next || !current->next->value)
			{
				ft_putendl_fd("minishell: syntax error near unexpected token",
					2);
				return (-1);
			}
			if (open(current->next->value, O_RDONLY) == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(current->next->value, 2);
				ft_putendl_fd(": No such file or directory", 2);
				return (-1);
			}
		}
		current = current->next;
	}
	return (0);
}

static int	execute_builtin(t_minishell *minishell, t_command *cmd)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (handle_redirections(cmd))
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		minishell->exit_code = 1;
		return (minishell->exit_code);
	}
	execute_command(minishell, cmd);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (minishell->exit_code);
}

static int	execute_pipeline(t_minishell *minishell, t_command *cmd)
{
	int		prev_pipe_in;
	pid_t	last_pid;

	prev_pipe_in = -1;
	last_pid = -1;
	while (cmd)
	{
		if (process_command_in_pipeline(minishell,
				&cmd, &prev_pipe_in, &last_pid))
			return (1);
	}
	wait_for_all_children(minishell, last_pid);
	return (minishell->exit_code);
}

int	command_executer(t_minishell *minishell)
{
	t_command	*cmd;
	int			num_commands;

	cmd = minishell->commands;
	num_commands = count_commands(cmd);
	if (num_commands == 1 && is_builtin(cmd->args[0]))
		return (execute_builtin(minishell, cmd));
	execute_pipeline(minishell, cmd);
	return (minishell->exit_code);
}
