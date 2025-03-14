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

static int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}
static int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, ECHO_LOWER) == 0
		|| ft_strcmp(cmd, CD_LOWER) == 0
		|| ft_strcmp(cmd, PWD_LOWER) == 0
		|| ft_strcmp(cmd, EXPORT_LOWER) == 0
		|| ft_strcmp(cmd, UNSET_LOWER) == 0
		|| ft_strcmp(cmd, ENV_LOWER) == 0
		|| ft_strcmp(cmd, EXIT_LOWER) == 0);
}

static void execute_command(t_minishell *minishell, t_command *cmd)
{
	if (ft_strcmp(cmd->args[0], ECHO_LOWER) == 0)
		builtin_echo(minishell, cmd);
	else if (ft_strcmp(cmd->args[0], CD_LOWER) == 0)
		builtin_cd(minishell, cmd);
	else if (ft_strcmp(cmd->args[0], PWD_LOWER) == 0)
		builtin_pwd(minishell, cmd);
	else if (ft_strcmp(cmd->args[0], EXPORT_LOWER) == 0)
		builtin_export(minishell, cmd);
	else if (ft_strcmp(cmd->args[0], UNSET_LOWER) == 0)
		builtin_unset(minishell, cmd);
	else if (ft_strcmp(cmd->args[0], ENV_LOWER) == 0)
		builtin_env(minishell, cmd);
	else if (ft_strcmp(cmd->args[0], EXIT_LOWER) == 0)
		builtin_exit(minishell, cmd);
	else
		command_process(minishell, cmd);
}

static void	cleanup_fds(int fd1, int fd2)
{
	if (fd1 != 0)
		close(fd1);
	if (fd2 != 0)
		close(fd2);
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

static int	count_commands(t_command *cmd)
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

static int	setup_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		return (1);
	}
	return (0);
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

static int	handle_parent_process(int prev_pipe_in, int pipe_fd[2], t_command **cmd)
{
	if (prev_pipe_in != 0)
		close(prev_pipe_in);
	close(pipe_fd[1]);
	*cmd = (*cmd)->next;
	return (pipe_fd[0]);
}

static void	wait_for_children(t_minishell *minishell)
{
	pid_t	pid;
	int		status;

	while ((pid = waitpid(-1, &status, 0)) > 0)
		;
	if (WIFEXITED(status))
	{
		minishell->exit_code = WEXITSTATUS(status);
		printf("Exit status of the child was %d\n", minishell->exit_code);
	}
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
