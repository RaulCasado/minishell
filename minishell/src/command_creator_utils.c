/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_creator_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 18:40:43 by racasado          #+#    #+#             */
/*   Updated: 2025/03/14 18:42:57 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*copy_or_null(char *file)
{
	char	*copy;

	copy = NULL;
	if (file)
		copy = ft_strdup(file);
	return (copy);
}

void	reset_command_info(t_command_info *ci)
{
	ci->args = NULL;
	ci->infile = NULL;
	ci->outfile = NULL;
	ci->append = 0;
	ci->pipe_in = 0;
	ci->pipe_out = 0;
}

t_command	*create_command(t_command_info *ci, t_command *next)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = ci->args;
	cmd->infile = copy_or_null(ci->infile); // what if ft_strdup fails
	cmd->outfile = copy_or_null(ci->outfile); // what if ft_strdup fails
	cmd->append = ci->append;
	cmd->pipe_in = ci->pipe_in;
	cmd->pipe_out = ci->pipe_out;
	cmd->next = next;
	reset_command_info(ci);
	return (cmd);
}

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
