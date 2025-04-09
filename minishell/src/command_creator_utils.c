/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_creator_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: droura-s <droura-s@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 18:40:43 by racasado          #+#    #+#             */
/*   Updated: 2025/04/09 11:54:04 by droura-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_command_info(t_command_info *ci)
{
	ci->args = NULL;
	ci->infile = NULL;
	ci->outfile = NULL;
	ci->append = 0;
	ci->pipe_in = 0;
	ci->pipe_out = 0;
	ci->extra_outfiles = NULL;
	ci->extra_count = 0;
}

t_command	*create_command(t_command_info *ci, t_command *next)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = ci->args;
	cmd->infile = ci->infile;
	cmd->outfile = ci->outfile;
	cmd->append = ci->append;
	cmd->pipe_in = ci->pipe_in;
	cmd->pipe_out = ci->pipe_out;
	cmd->extra_outfiles = ci->extra_outfiles;
	cmd->extra_count = ci->extra_count;
	cmd->next = next;
	reset_command_info(ci);
	return (cmd);
}
