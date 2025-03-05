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
// btw we should check the fd of every builtin funciton since the
// redirections are handled in the execute_command function
// instead of the builtin functions themselves for example
// (no se como explicarme en ingles pero lo que teniamos de si el fd no es 1 xd y cosas asi) done i think check it later

char command_executer(t_minishell *minishell)
{
	t_command *cmd = minishell->commands;
	int pipe_fd[2], prev_pipe_in = 0, num_commands = 0;
	pid_t pid;
	int	status;

	// Count commands
	t_command *tmp = cmd;
	while (tmp)
	{
		num_commands++;
		tmp = tmp->next;
	}

	// Single builtin: execute in parent
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

	// Pipeline handling
	while (cmd)
	{
		if (cmd->next && pipe(pipe_fd) == -1)
			return (perror("minishell: pipe"), 1);

		pid = fork();
		if (pid == -1)
			return (perror("minishell: fork"), 1);
		else if (pid == 0) // Child
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
		}
		else // Parent
		{
			if (prev_pipe_in != 0)
				close(prev_pipe_in);
			if (cmd->next)
				prev_pipe_in = pipe_fd[0];
			else
				prev_pipe_in = 0;
			close(pipe_fd[1]);
			cmd = cmd->next;
		}
	}

	// Wait for all children
	while (waitpid(pid, &status, 0) > 0);

	// echo "$?"
	if (WIFEXITED(status))
	{
		// minishell->exit_code = WEXITSTATUS(status);
		minishell->exit_code = WEXITSTATUS(status);
		printf("Exit status of the child was %d\n", minishell->exit_code);
	}
	return (0);
}
