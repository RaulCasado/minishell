/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:32:48 by racasado          #+#    #+#             */
/*   Updated: 2025/03/14 17:32:48 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_output_file(char *file, int append)
{
	int	fd;

	if (append == 2)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (append == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		return (1);
	if (fd == -1)
	{
		perror("Minishell: open");
		return (1);
	}
	return (fd);
}

// New helper: create a tee redirection for multiple output files.
static int	setup_tee_redirection(t_command *cmd)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
	{
		perror("Minishell: pipe");
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("Minishell: fork");
		return (-1);
	}
	if (pid == 0)
	{
		/* Child process acts as tee */
		close(pipefd[1]);
		{
			char	buffer[1024];
			int		n;
			int		fd_main = open_output_file(cmd->outfile, cmd->append);
			if (fd_main < 0)
				exit(1);
			// Allocate array for extra files.
			int	*extra_fds = malloc(sizeof(int) * cmd->extra_count);
			for (int i = 0; i < cmd->extra_count; i++)
			{
				extra_fds[i] = open_output_file(cmd->extra_outfiles[i], cmd->append);
				// In a real-world case, check for errors here.
			}
			while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0)
			{
				write(fd_main, buffer, n);
				for (int i = 0; i < cmd->extra_count; i++)
					write(extra_fds[i], buffer, n);
			}
			close(fd_main);
			for (int i = 0; i < cmd->extra_count; i++)
				close(extra_fds[i]);
			free(extra_fds);
		}
		exit(0);
	}
	/* Parent process: use pipe write end for command stdout */
	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("Minishell: dup2");
		close(pipefd[1]);
		return (-1);
	}
	close(pipefd[1]);
	return (0);
}

static char	handle_outfile(t_command *cmd)
{
	if (cmd->outfile[0] == '\0' || (unsigned char)cmd->outfile[0] > 127)
		return (1);
	// If there are extra outfiles, set up tee redirection.
	if (cmd->extra_count > 0)
	{
		if (setup_tee_redirection(cmd) < 0)
			return (1);
		return (0);
	}
	int	fd = open_output_file(cmd->outfile, cmd->append);
	if (fd == -1)
		return (1);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Minishell: dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static char	handle_infile(t_command *cmd)
{
	int	fd;

	fd = open(cmd->infile, O_RDONLY); // infile < echo <--- blow up
	if (fd == -1)
	{
		perror("Minishell: open");
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("Minishell: dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	handle_redirections(t_command *cmd)
{
	if (cmd->outfile)
	{
		if (handle_outfile(cmd))
			return (-1);
	}
	if (cmd->infile)
	{
		if (handle_infile(cmd))
			return (-1);
	}
	// Minishell> echo hola > outfile
	// Minishell> cat outfile
	return (0);
}
