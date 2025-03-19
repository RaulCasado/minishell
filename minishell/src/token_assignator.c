/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_assignator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: droura-s <droura-s@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:33:05 by racasado          #+#    #+#             */
/*   Updated: 2025/03/19 13:20:24 by droura-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*create_token(char *value)
{
	t_token	*token;

	if (!value)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (ft_strncmp(value, "|", 1) == 0)
		token->type = TOKEN_PIPE;
	else if (ft_strncmp(value, ">>", 2) == 0)
		token->type = TOKEN_REDIR_APPEND;
	else if (ft_strncmp(value, "<<", 2) == 0)
		token->type = TOKEN_HEREDOC;
	else if (ft_strncmp(value, "<", 1) == 0)
		token->type = TOKEN_REDIR_IN;
	else if (ft_strncmp(value, ">", 1) == 0)
		token->type = TOKEN_REDIR_OUT;
	else
		token->type = TOKEN_WORD;
	token->value = value;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **tokens, char *value)
{
	t_token	*last;
	t_token	*new_token;

	new_token = create_token(value);
	if (!new_token)
		return ;
	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	last = *tokens;
	while (last->next)
		last = last->next;
	last->next = new_token;
}

const char	*token_type_to_string(int type)
{
	if (type == TOKEN_WORD)
		return ("TOKEN_WORD");
	if (type == TOKEN_PIPE)
		return ("TOKEN_PIPE");
	if (type == TOKEN_REDIR_IN)
		return ("TOKEN_REDIR_IN");
	if (type == TOKEN_REDIR_OUT)
		return ("TOKEN_REDIR_OUT");
	if (type == TOKEN_REDIR_APPEND)
		return ("TOKEN_REDIR_APPEND");
	if (type == TOKEN_HEREDOC)
		return ("TOKEN_HEREDOC");
	return ("UNKNOWN_TOKEN");
}

void	print_tokens(t_token *tokens)
{
	t_token	*token_ptr;
	size_t	i;

	if (!tokens)
		return ;
	i = 0;
	token_ptr = tokens;
	printf("\nPrinting tokens:\n");
	while (token_ptr)
	{
		printf("%zd: %s\n", i, token_ptr->value);
		//printf("Type: %s\n", token_type_to_string(token_ptr->type));
		token_ptr = token_ptr->next;
		i++;
	}
	printf("\n");
}
