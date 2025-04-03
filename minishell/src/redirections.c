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

static char	handle_outfile(t_command *cmd)
{
    if (cmd->outfile[0] == '\0' || (unsigned char)cmd->outfile[0] > 127)
        return (1);
    // Open the main output file regardless of extra redirections.
    int	fd = open_output_file(cmd->outfile, cmd->append);
    if (fd == -1)
        return (1);
    // For each extra outfile, try opening it to match bash semantics.
    for (int i = 0; i < cmd->extra_count; i++)
    {
        int extra_fd = open_output_file(cmd->extra_outfiles[i], cmd->append);
        if (extra_fd == -1)
        {
            perror("Minishell: open");
            close(fd);
            return (1);
        }
        close(extra_fd);
    }
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
	if (cmd->outfile)
	{
		if (handle_outfile(cmd))
			return (-1);
	}
	if (cmd->infile)
	{
		if (handle_infile(cmd))
		{
			perror("Minishell: ");
			return (-1);
		}
	}
	// Minishell> echo hola > outfile
	// Minishell> cat outfile
	return (0);
}
