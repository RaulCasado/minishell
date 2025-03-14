/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_builder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:33:10 by racasado          #+#    #+#             */
/*   Updated: 2025/03/14 17:33:11 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
