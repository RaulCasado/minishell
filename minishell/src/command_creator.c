/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_creator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:30:13 by racasado          #+#    #+#             */
/*   Updated: 2025/03/14 18:08:57 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**add_arg(char **args, char *new_arg)
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
	new_args[count] = strdup(new_arg);
	new_args[count + 1] = NULL;

	free(args);
	return (new_args);
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
	cmd->next = next;
	return (cmd);
}

static void	handle_redirection(t_token **t, t_command_info *ci, int type)
{
	if (!((*t)->next))
		return ;
	*t = (*t)->next;
	if (type == TOKEN_REDIR_IN)
		ci->infile = (*t)->value;
	else if (type == TOKEN_REDIR_OUT)
	{
		ci->outfile = (*t)->value;
		ci->append = 1;
	}
	else if (type == TOKEN_REDIR_APPEND)
	{
		ci->outfile = (*t)->value;
		ci->append = 2;
	}
}

static void	handle_pipe(t_command **cl, t_command_info *ci, t_command **curr)
{
	ci->pipe_out = 1;
	if (*cl)
		*curr = create_command(ci, *curr);
	else
	{
		*curr = create_command(ci, NULL);
		*cl = *curr;
	}
	ci->args = NULL;
	ci->infile = NULL;
	ci->outfile = NULL;
	ci->append = 0;
	ci->pipe_in = 1;
	ci->pipe_out = 0;
}

static void	handle_word(t_command_info *ci, char *value)
{
	ci->args = add_arg(ci->args, value);
}

static void process_token(t_token *t, t_command_info *ci,
		t_command **cl, t_command **curr)
{
	if (t->type == TOKEN_WORD)
		handle_word(ci, t->value);
	else if (t->type >= TOKEN_REDIR_IN && t->type <= TOKEN_REDIR_APPEND)
		handle_redirection(&t, ci, t->type);
	else if (t->type == TOKEN_PIPE)
		handle_pipe(cl, ci, curr);
}

t_command *parse_tokens(t_token *t)
{
	t_command *cl = NULL, *curr = NULL;
	t_command_info ci = {NULL, NULL, NULL, 0, 0, 0};

	while (t)
	{
		process_token(t, &ci, &cl, &curr);
		t = t->next;
	}
	if (cl)
		curr->next = create_command(&ci, NULL);
	else
		cl = create_command(&ci, NULL);
	return (cl);
}
