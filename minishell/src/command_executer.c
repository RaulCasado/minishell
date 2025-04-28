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
		better_close(saved_stdin);
		better_close(saved_stdout);
		minishell->exit_code = 1;
		return (minishell->exit_code);
	}
	execute_command(minishell, cmd);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	better_close(saved_stdin);
	better_close(saved_stdout);
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
		if (!cmd->args)
		{
			cmd = cmd->next;
			continue ;
		}
		if (ce_process_command_in_pipeline(minishell,
				&cmd, &prev_pipe_in, &last_pid))
			return (1);
	}
	ce_wait_for_all_children(minishell, last_pid);
	return (minishell->exit_code);
}

static int	check_input_redirections(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIR_IN)
		{
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

int	command_executer(t_minishell *minishell)
{
	t_command	*cmd;
	int			num_commands;

	cmd = minishell->commands;
	num_commands = count_commands(cmd);
	if (check_input_redirections(minishell->tokens) == -1)
	{
		if (cmd->next)
		{
			cmd = cmd->next;
			if (!cmd->args[1] && !cmd->infile)
				cmd->infile = ft_strdup("/dev/null");
		}
		else
			return (1);
	}
	if (num_commands == 1 && !cmd->args)
		return (0);
	if (num_commands == 1 && is_builtin(cmd->args[0]))
		return (execute_builtin(minishell, cmd));
	return (execute_pipeline(minishell, cmd));
}
