/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_unquoter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: droura-s <droura-s@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:33:52 by racasado          #+#    #+#             */
/*   Updated: 2025/03/26 14:18:53 by droura-s         ###   ########.fr       */
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
	// printf("\t%s%s\n\n", head, tail);
	new_value = ft_strjoin(head, tail);
	free(head);
	free(tail);
	return (new_value);
}

static char	*loop_subtoken(char *value, char mark_type)
{
	size_t	i;
	size_t	j;

	// printf("\tloop_subtoken: %s\n", value);
	while (1)
	{
		i = 0;
		j = ft_strlen(value) - 1;
		// printf("\t%s\n", value);
		while (value[i] && value[i] != mark_type)
			i++;
		while (j > 0 && value[j] && value[j] != mark_type)
			j--;
		// printf("\t%zd:%c %zd:%c\n", i, value[i], j, value[j]);
		if (i < j)
		{
			value = remove_one_quote(value, i);
			// printf("\t\t%s\n", value);
			value = remove_one_quote(value, j - 1);
			// printf("\t\t%s\n", value);
			continue ;
		}
		break ;
	}
	return (value);
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
			current->value = new_value;
		}
		current->value = loop_subtoken(current->value, DOUBLE_MARK); // FREE OLD VALUE
		current->value = loop_subtoken(current->value, SIMPLE_MARK);
		current = current->next;
	}
	// print_tokens(*tokens);
}
