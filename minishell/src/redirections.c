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
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(file, 2);
		if (errno == EISDIR)
			ft_putendl_fd(": Is a directory", 2);
		else if (errno == EACCES)
			ft_putendl_fd(": Permission denied", 2);
		else
			perror("");
		return (-1);
	}
	return (fd);
}

static int	check_extra_outfiles(t_command *cmd, int fd, int append)
{
	int	i;
	int	extra_fd;

	i = 0;
	while (i < cmd->extra_count)
	{
		extra_fd = open_output_file(cmd->extra_outfiles[i], append);
		if (extra_fd == -1)
		{
			close(fd);
			return (1);
		}
		close(extra_fd);
		i++;
	}
	return (0);
}

static char	handle_outfile(t_command *cmd)
{
	int	fd;

	if (cmd->outfile[0] == '\0' || (unsigned char)cmd->outfile[0] > 127)
		return (1);
	fd = open_output_file(cmd->outfile, cmd->append);
	if (fd == -1)
		return (1);
	if (check_extra_outfiles(cmd, fd, cmd->append))
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

	fd = open(cmd->infile, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(cmd->infile, 2);
		if (errno == EACCES)
			ft_putendl_fd(": Permission denied", 2);
		else
			ft_putendl_fd(": No such file or directory", 2);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	handle_redirections(t_command *cmd)
{
	int	result;

	result = 0;
	if (cmd->outfile)
	{
		if (handle_outfile(cmd))
			result = -1;
	}
	if (cmd->infile)
	{
		if (handle_infile(cmd))
			result = -1;
	}
	return (result);
}
