/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_unquoter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:33:52 by racasado          #+#    #+#             */
/*   Updated: 2025/04/22 11:14:23 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*remove_one_quote(char *value, size_t i)
{
	char	*head;
	char	*tail;
	char	*new_value;

	head = ft_substr(value, 0, i);
	if (!head)
		return (NULL);
	tail = ft_substr(value, i + 1, ft_strlen(value) - i + 1);
	if (!tail)
	{
		free(head);
		return (NULL);
	}
	new_value = ft_strjoin(head, tail);
	free(head);
	free(tail);
	return (new_value);
}

static char	*loop_subtoken(t_token *token, char mark_type)
{
	size_t	i;
	size_t	j;

	while (1)
	{
		if (ft_strlen(token->value) < 2)
			break ;
		i = 0;
		j = ft_strlen(token->value) - 1;
		while (token->value[i] && token->value[i] != mark_type)
			i++;
		while (j > 0 && token->value[j] && token->value[j] != mark_type)
			j--;
		if (i >= j)
			break ;
		swap_free(&token->value, remove_one_quote(token->value, i));
		if (!token->value)
			return (NULL);
		swap_free(&token->value, remove_one_quote(token->value, j - 1));
		if (!token->value)
			return (NULL);
	}
	return (token->value);
}

static char	*unquote_token(t_token *token, size_t value_len)
{
	char	*new_value;

	if (value_len <= 2)
	{
		free(token->value);
		return (ft_strdup(""));
	}
	new_value = ft_strtrim(token->value, " \n\t\r\f");
	free(token->value);
	return (new_value);
}

static void	process_outer_quotes(t_token *curr)
{
	char	*new_value;

	if (curr->type == TOKEN_WORD
		&& (get_global_marks(curr->value, SIMPLE_MARK)
			|| get_global_marks(curr->value, DOUBLE_MARK)))
	{
		new_value = unquote_token(curr, ft_strlen(curr->value));
		if (!new_value)
		{
			perror("Minishell: malloc failed in unquoter");
			exit(3);
		}
		curr->value = new_value;
	}
}

void	unquoter(t_token **tokens)
{
	t_token	*current;
	t_token	*prev;

	current = *tokens;
	prev = NULL;
	while (current)
	{
		if (prev && prev->type == TOKEN_HEREDOC)
		{
			prev = current;
			current = current->next;
			continue ;
		}
		process_outer_quotes(current);
		swap_free(&current->value, loop_subtoken(current, DOUBLE_MARK));
		swap_free(&current->value, loop_subtoken(current, SIMPLE_MARK));
		prev = current;
		current = current->next;
	}
}
