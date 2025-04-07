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

static void setup_child_process(t_minishell *minishell, t_command *cmd,
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
		// Manejo de redirecciones: si falla, este hijo hace exit(1)
		if (handle_redirections(cmd))
			exit(1);
		execute_command(minishell, cmd);
		exit(EXIT_SUCCESS);
	}

static int create_process(t_minishell *minishell, t_command **cmd,
						  int *prev_pipe_in, int pipe_fd[2])
{
	pid_t pid;
	int new_pipe;

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

void wait_for_all_children(t_minishell *minishell, pid_t last_pid)
{
    pid_t pid;
    int status;
    int last_status = 0;
    while ((pid = wait(&status)) > 0)
    {
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

static int execute_pipeline(t_minishell *minishell, t_command *cmd)
{
    int pipe_fd[2];
    int prev_pipe_in;
    pid_t   last_pid = -1;
    pid_t   pid;
   
    pipe_fd[0] = -1;
    pipe_fd[1] = -1;
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
            pipe_fd[0] = -1;
            pipe_fd[1] = -1;
        }
        pid = fork();
        if (pid == -1)
            return (perror("minishell: fork"), 1);
        else if (pid == 0)
        {
            setup_child_process(minishell, cmd, prev_pipe_in, pipe_fd);
            // setup_child_process() no regresa.
        }
        else
        {
            // Actualiza last_pid para que sea el pid del último forked.
            last_pid = pid;
            if (cmd->next)
            {
                int new_pipe = handle_parent_process(prev_pipe_in, pipe_fd, &cmd);
                if (new_pipe >= 0)
                    prev_pipe_in = new_pipe;
                else
                    prev_pipe_in = 0;
            }
            else
            {
                cmd = cmd->next;
                if (prev_pipe_in > 0)
                {
                    cleanup_fds(prev_pipe_in, 0);
                    prev_pipe_in = 0;
                }
            }
        }
    }
    wait_for_all_children(minishell, last_pid);
    return (minishell->exit_code);
}


static int check_output_redirections(t_token *tokens)
{
	t_token *current = tokens;
	int fd;

	while (current)
	{
		if (current->type == TOKEN_REDIR_OUT || current->type == TOKEN_REDIR_APPEND)
		{
			if (!current->next || !current->next->value)
			{
				ft_putendl_fd("minishell: syntax error near unexpected token", 2);
				return (-1);
			}
			if (current->type == TOKEN_REDIR_OUT)
				fd = open(current->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				fd = open(current->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(current->next->value, 2);
				ft_putendl_fd(": No such file or directory", 2);
				return (-1);
			}
			close(fd);
		}
		current = current->next;
	}
	return (0);
}

/*
 * Recorre la lista de tokens y, para cada operador de redirección de entrada,
 * intenta abrir el archivo asociado. Si falla, imprime el error y retorna -1.
 */
static int check_input_redirections(t_token *tokens)
{
	t_token *current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIR_IN) // Asumiendo que TOKEN_REDIR_IN está definido
		{
			if (!current->next || !current->next->value)
			{
				ft_putendl_fd("minishell: syntax error near unexpected token", 2);
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

int command_executer(t_minishell *minishell)
{
	t_command *cmd;
	int num_commands;
	int saved_stdin;
	int saved_stdout;

	cmd = minishell->commands;
	if (check_input_redirections(minishell->tokens) == -1 || check_output_redirections(minishell->tokens) == -1)
		exit(1);
	num_commands = count_commands(cmd);
	// Single builtin: execute in parent with proper fd backup/restoration
	if (num_commands == 1 && is_builtin(cmd->args[0]))
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		if (handle_redirections(cmd))
			exit(EXIT_FAILURE);
		execute_command(minishell, cmd);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (minishell->exit_code);
	}
	execute_pipeline(minishell, cmd);
	return (minishell->exit_code);
}
