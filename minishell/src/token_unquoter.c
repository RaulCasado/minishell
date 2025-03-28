/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_unquoter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:33:52 by racasado          #+#    #+#             */
/*   Updated: 2025/03/28 11:25:54 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	swap_free(char **original, char *new)
{
	if (!*original || !new || *original == new)
		return ;
	free(*original);
	*original = new;
}

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
	new_value = malloc(sizeof(char) * (value_len - 1));
	if (!new_value)
	{
		free(token->value);
		return (NULL);
	}
	ft_memcpy(new_value, token->value + 1, value_len - 2);
	new_value[value_len - 2] = '\0';
	free(token->value);
	return (new_value);
}

void	unquoter(t_token **tokens)
{
	t_token	*current;
	char	*new_value;

	current = *tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD
			&& (current->value[0] == DOUBLE_MARK
				|| current->value[0] == SIMPLE_MARK))
		{
			new_value = unquote_token(current, ft_strlen(current->value));
			if (!new_value)
			{
				perror("Minishell: malloc failed in unquoter");
				exit(3);
			}
			current->value = new_value; // NO NULL CHECKS EN LOS SWAPS??
		}
		swap_free(&current->value, loop_subtoken(current, DOUBLE_MARK));
		swap_free(&current->value, loop_subtoken(current, SIMPLE_MARK));
		current = current->next;
	}
	// print_tokens(*tokens);
}
