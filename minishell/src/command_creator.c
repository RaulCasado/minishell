/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_creator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:30:13 by racasado          #+#    #+#             */
/*   Updated: 2025/04/07 22:02:28 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirection(t_token **t, t_command_info *ci)
{
	t_token	*next;

	next = (*t)->next;
	if (!next)
		return ;
	if ((*t)->type == TOKEN_REDIR_IN)
		handle_redir_in(ci, next);
	else if ((*t)->type == TOKEN_REDIR_OUT)
		handle_redir_out(ci, next, 1);
	else if ((*t)->type == TOKEN_REDIR_APPEND)
		handle_redir_out(ci, next, 2);
}

void	handle_pipe(t_command **cl, t_command_info *ci, t_command **curr)
{
	ci->pipe_out = 1;
	if (*cl)
	{
		(*curr)->next = create_command(ci, NULL);
		*curr = (*curr)->next;
	}
	else
	{
		*cl = create_command(ci, NULL);
		*curr = *cl;
	}
	reset_command_info(ci);
	ci->pipe_in = 1;
}

void	handle_word(t_command_info *ci, char *value)
{
	ci->args = add_arg(ci->args, value);
}

t_token	*process_token(t_token *t, t_command_info *ci,
		t_command **cl, t_command **curr)
{
	if (t->type == TOKEN_WORD)
	{
		handle_word(ci, t->value);
		t = t->next;
	}
	else if (t->type >= TOKEN_REDIR_IN && t->type <= TOKEN_REDIR_APPEND)
	{
		handle_redirection(&t, ci);
		if (t && t->next)
			t = t->next->next;
		else
			t = NULL;
	}
	else if (t->type == TOKEN_PIPE)
	{
		handle_pipe(cl, ci, curr);
		t = t->next;
	}
	return (t);
}

t_command	*parse_tokens(t_token *t)
{
	t_command		*cl;
	t_command		*curr;
	t_command		*last_cmd;
	t_command_info	ci;

	cl = NULL;
	curr = NULL;
	reset_command_info(&ci);
	while (t)
		t = process_token(t, &ci, &cl, &curr);
	if (ci.args || ci.infile || ci.outfile)
	{
		last_cmd = create_command(&ci, NULL);
		if (!cl)
			cl = last_cmd;
		else
			curr->next = last_cmd;
	}
	return (cl);
}
