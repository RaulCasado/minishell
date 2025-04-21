/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:00:00 by racasado          #+#    #+#             */
/*   Updated: 2025/04/21 17:56:13 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_temp_file(void)
{
	int		i;
	char	*filename;
	char	*num_str;
	int		fd;

	i = 0;
	while (1)
	{
		num_str = ft_itoa(i);
		if (!num_str)
			return (NULL);
		filename = ft_strjoin("/tmp/minishell_heredoc_", num_str);
		free(num_str);
		if (!filename)
			return (NULL);
		fd = open(filename, O_RDWR | O_CREAT | O_EXCL, 0644);
		if (fd != -1)
		{
			close(fd);
			return (filename);
		}
		free(filename);
		i++;
	}
	return (NULL);
}

static int	read_heredoc_input(char *delimiter, int fd)
{
	char		*line;
	size_t		delimiter_len;

	delimiter_len = ft_strlen(delimiter);
	ft_putstr_fd("> ", 1);
	while (1)
	{
		line = readline("");
		if (!line)
		{
			ft_putstr_fd("minishell: warning\n", 2);
			break ;
		}
		if (ft_strlen(line) == delimiter_len &&
			ft_strncmp(line, delimiter, delimiter_len) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
		ft_putstr_fd("> ", 1);
	}
	return (0);
}

char	*handle_heredoc(char *delimiter)
{
	char	*filename;
	int		fd;

	filename = create_temp_file();
	if (!filename)
		return (NULL);
	
	fd = open(filename, O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(filename);
		return (NULL);
	}
	
	read_heredoc_input(delimiter, fd);
	close(fd);
	
	return (filename);
}
