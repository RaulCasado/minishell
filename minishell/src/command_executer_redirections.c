/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executer_redirections.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 22:18:16 by racasado          #+#    #+#             */
/*   Updated: 2025/04/07 22:23:25 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ce_process_command_in_pipeline(t_minishell *minishell, t_command **cmd,
	int *prev_pipe_in, pid_t *last_pid)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (ce_init_pipe(*cmd, pipe_fd))
		return (1);
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	else if (pid == 0)
		ce_setup_child_process(minishell, *cmd, *prev_pipe_in, pipe_fd);
	else
	{
		*last_pid = pid;
		ce_process_parent(cmd, prev_pipe_in, pipe_fd);
	}
	return (0);
}

int	ce_handle_single_output_redirection(t_token *token)
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

int	ce_check_output_redirections(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND)
		{
			if (ce_handle_single_output_redirection(current) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}

int	ce_check_input_redirections(t_token *tokens)
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
