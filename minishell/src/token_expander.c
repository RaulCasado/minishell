/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:33:35 by racasado          #+#    #+#             */
/*   Updated: 2025/03/30 21:55:39 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_loop(t_token *token, t_minishell *minishell,
	ssize_t *i)
{
	ssize_t	j;

	(*i)++;
	if (!token->value[*i] || token->value[*i] == ' ')
		return (0);
	j = *i;
	while (token->value[j + 1] && token->value[j + 1] != ' '
		&& token->value[j + 1] != DOUBLE_MARK
		&& token->value[j + 1] != SIMPLE_MARK
		&& token->value[j] != QUESTION_MARK)
		j++;
	if (token->value[*i] != DOUBLE_MARK)
	{
		token->value = expand_variable(token->value, *i, j, minishell);
		(*i) = 0;
	}
	if (!token->value)
		return (0);
	return (1);
}

char	get_global_marks(char *ptr, char mark_type)
{
	return (ptr[0] == mark_type
		&& ptr[ft_strlen(ptr) - 1] == mark_type);
}

void	expand_tokens(t_token **tokens, t_minishell *minishell)
{
	t_token	*current;
	ssize_t	i;

	current = *tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			if (get_global_marks(current->value, SIMPLE_MARK))
			{
				current = current->next;
				continue ;
			}
			i = -1;
			while (current->value[++i])
			{
				if (current->value[i] == DOLLAR)
				{
					if (!expand_loop(current, minishell, &i))
						break ;
				}
			}
		}
		current = current->next;
	}
}
