/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_unquoter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:33:52 by racasado          #+#    #+#             */
/*   Updated: 2025/03/14 17:33:52 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		current = current->next;
	}
}
