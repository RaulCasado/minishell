/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_builder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:33:10 by racasado          #+#    #+#             */
/*   Updated: 2025/04/07 11:29:20 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_marks(char chr, char *mark_d, char *mark_s)
{
    // Si se encuentra una comilla doble y no estamos dentro de comillas simples, cambiar estado.
    if (chr == DOUBLE_MARK && !(*mark_s))
        *mark_d = !(*mark_d);
    // Si se encuentra una comilla simple y no estamos dentro de comillas dobles, cambiar estado.
    else if (chr == SIMPLE_MARK && !(*mark_d))
        *mark_s = !(*mark_s);
}

ssize_t	ft_symbol_len(char *ptr)
{
	if (*ptr == '|')
		return (1);
	if (*ptr == '>')
	{
		if (*(ptr + 1) == '>')
			return (2);
		return (1);
	}
	else if (*ptr == '<')
	{
		if (*(ptr + 1) == '<')
			return (2);
		return (1);
	}
	return (0);
}

t_token	*tokenize_input(char *input, t_minishell *minishell)
{
	t_token	*tokens;

	tokens = NULL;
	if (!split_input(-1, input, &tokens))
		return (NULL);
	if (!tokens)
		return (NULL);
	expand_tokens(&tokens, minishell);
	if (!tokenize_check(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	unquoter(&tokens);
	return (tokens);
}
