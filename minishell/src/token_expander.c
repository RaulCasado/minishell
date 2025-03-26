/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: droura-s <droura-s@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:33:35 by racasado          #+#    #+#             */
/*   Updated: 2025/03/26 12:26:13 by droura-s         ###   ########.fr       */
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
		&& token->value[j + 1] != DOUBLE_MARK)
		j++;
	token->value = expand_variable(token->value, *i, j, minishell);
	(*i) = 0;
	if (!token->value)
		return (0);
	return (1);
}

char	closed_quotes(char *ptr)
{
	while (*ptr)
		if (*ptr++ == DOUBLE_MARK)
			return (1);
	return (0);
}

void	expand_tokens(t_token **tokens, t_minishell *minishell)
{
	t_token	*current;
	ssize_t	i;
	char	marks;

	marks = 0;
	current = *tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			i = -1;
			while (current->value[++i])
			{
				handle_marks(current->value[i], &marks, NULL, 0);
				if (marks && current->value[i] == DOLLAR
					&& closed_quotes(current->value + i))
					if (!expand_loop(current, minishell, &i))
						break ;
			}
		}
		current = current->next;
	}
}
