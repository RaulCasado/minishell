/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_creator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:30:13 by racasado          #+#    #+#             */
/*   Updated: 2025/04/07 20:45:49 by racasado         ###   ########.fr       */
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

static int	immediate_open_output_file(char *file, int append)
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
static void	handle_redir_in(t_command_info *ci, t_token *next)
{
	free(ci->infile);
	ci->infile = ft_strdup(next->value);
}

/*
** Helper para TOKEN_REDIR_OUT y TOKEN_REDIR_APPEND.
** 'append' vale 1 para '>' y 2 para '>>'.
*/
static void	handle_redir_out(t_command_info *ci, t_token *next, int append)
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
