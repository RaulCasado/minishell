/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_creator_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 21:59:01 by racasado          #+#    #+#             */
/*   Updated: 2025/04/07 22:00:07 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**add_arg(char **args, char *new_arg)
{
	int		count;
	int		i;
	char	**new_args;

	count = 0;
	i = 0;
	while (args && args[count])
		count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (NULL);
	while (i < count)
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[count] = ft_strdup(new_arg);
	new_args[count + 1] = NULL;
	free(args);
	return (new_args);
}

int	immediate_open_output_file(char *file, int append)
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

/*
** Helper para TOKEN_REDIR_IN
*/
void	handle_redir_in(t_command_info *ci, t_token *next)
{
	free(ci->infile);
	ci->infile = ft_strdup(next->value);
}

/*
** Helper para TOKEN_REDIR_OUT y TOKEN_REDIR_APPEND.
** 'append' vale 1 para '>' y 2 para '>>'.
*/
void	handle_redir_out(t_command_info *ci, t_token *next, int append)
{
	int	fd;

	fd = immediate_open_output_file(next->value, append);
	if (fd >= 0)
		close(fd);
	if (ci->outfile)
	{
		ci->extra_outfiles = add_arg(ci->extra_outfiles, ci->outfile);
		ci->extra_count++;
		free(ci->outfile);
	}
	ci->outfile = ft_strdup(next->value);
	if (append == 2)
		ci->append = 2;
	else
		ci->append = 1;
}
