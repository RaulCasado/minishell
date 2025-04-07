/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 22:24:25 by racasado          #+#    #+#             */
/*   Updated: 2025/04/07 22:24:25 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (ce_process_command_in_pipeline(minishell,
				&cmd, &prev_pipe_in, &last_pid))
			return (1);
	}
	ce_wait_for_all_children(minishell, last_pid);
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
	return (execute_pipeline(minishell, cmd));
}
