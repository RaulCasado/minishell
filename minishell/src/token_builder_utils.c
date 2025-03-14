/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_builder_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: droura-s <droura-s@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 18:54:11 by droura-s          #+#    #+#             */
/*   Updated: 2025/03/14 18:54:11 by droura-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	build_and_add(char *ptr, size_t start, size_t len, t_token **tokens)
{
	char	*new_word;

	new_word = ft_substr(ptr, start, len);
	if (!new_word)
	{
		free_tokens(*tokens);
		*tokens = NULL;
		return (0);
	}
	add_token(tokens, new_word);
	return (1);
}

static int	handle_word(char *ptr, ssize_t *i, t_token **tokens)
{
	ssize_t	j;

	j = *i;
	while (ptr[*i + 1] != ' ' && ptr[*i + 1] != '\0'
		&& !ft_symbol_len(&ptr[*i + 1])
		&& ptr[*i + 1] != '"' && ptr[*i + 1] != '\'')
		(*i)++;
	return (build_and_add(ptr, j, *i - j + 1, tokens));
}

static int	handle_quotes(char *ptr, ssize_t *i, t_token **tokens)
{
	size_t	start;
	char	quote;

	quote = ptr[*i];
	start = (*i)++;
	while (ptr[*i] && ptr[*i] != quote)
		(*i)++;
	if (!ptr[*i])
	{
		printf("Syntax error: unclosed quotes\n");
		free_tokens(*tokens);
		return (0);
	}
	return (build_and_add(ptr, start, *i - start + 1, tokens));
}

int	split_input(ssize_t i, char *ptr, t_token **tokens)
{
	ssize_t	symbol_len;

	while (ptr[++i])
	{
		symbol_len = ft_symbol_len(&ptr[i]);
		if (symbol_len > 0)
		{
			if (!build_and_add(ptr, i, symbol_len, tokens))
				return (0); // Frees
			i += symbol_len - 1;
		}
		else if (ptr[i] == '\'' || ptr[i] == '"')
		{
			if (!handle_quotes(ptr, &i, tokens))
				return (0);
		}
		else if (ptr[i] != ' ')
		{
			if (!handle_word(ptr, &i, tokens))
				return (0);
		}
	}
	free(ptr);
	return (1);
}
