/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_builder_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: droura-s <droura-s@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 18:54:11 by droura-s          #+#    #+#             */
/*   Updated: 2025/03/19 12:58:54 by droura-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	build_and_add(char *ptr, size_t start, size_t len, t_token **tokens)
{
	char	*new_word;

	new_word = ft_substr(ptr, start, len);
	if (!new_word)
	{
		printf("FREEEEEEEING\n\n");
		free_tokens(*tokens);
		*tokens = NULL;
		return (0);
	}
	if (!ft_strlen(new_word)) // Fixing the heisenbug of ghost tokens in echo -n
		free(new_word);
	else
		add_token(tokens, new_word);
	return (1);
}

static int handle_word(char *ptr, ssize_t *i, t_token **tokens, char *mark_d, char *mark_s)
{
    ssize_t j;

	j = (*i);
    while (ptr[*i])
    {
        handle_marks(ptr[*i], mark_d, mark_s, 1);
        if (ptr[*i] == ' ' && !(*mark_d || *mark_s))
            break ;
        (*i)++;
    }
	if (*mark_d || *mark_s)
	{
		free_tokens(*tokens);
		*tokens = NULL;
		return (printf("Syntax error: unclosed quotes\n"), 0);
	}
    return (build_and_add(ptr, j, *i - j, tokens));
}

int split_input2(ssize_t i, char *ptr, t_token **tokens)
{
    ssize_t symbol_len;
    char mark_d = 0;
    char mark_s = 0;

    // Asegúrate de que la cadena no esté vacía
    if (!ptr)
        return (0);

    while (ptr[++i]) // Asegúrate de que `i` no supere el tamaño de la cadena
    {
        symbol_len = ft_symbol_len(&ptr[i]);

        if (symbol_len > 0)
        {
            if (!build_and_add(ptr, i, symbol_len, tokens))
                return (0);
            i += symbol_len - 1; // Ajustar `i` después de agregar un símbolo
        }
        else if (ptr[i] != ' ' || mark_d || mark_s)
        {
            // Llamar a handle_word solo si hay una palabra o comillas
            if (!handle_word(ptr, &i, tokens, &mark_d, &mark_s))
                return (0);
        }

        // Verifica que no hay desbordamiento de memoria antes de acceder
        if (i >= ft_strlen(ptr))
            break;
    }

    return (1);
}

int	split_input(ssize_t i, char *ptr, t_token **tokens)
{
	ssize_t	symbol_len;
	char	mark_d;
	char	mark_s;

	mark_d = 0;
	mark_s = 0;
	while (ptr[++i])
	{
		symbol_len = ft_symbol_len(&ptr[i]);
		if (symbol_len > 0)
		{
			if (!build_and_add(ptr, i, symbol_len, tokens))
				return (0);
			i += symbol_len - 1;
		}
		else if (ptr[i] != ' ' || mark_d || mark_s)
			if (!handle_word(ptr, &i, tokens, &mark_d, &mark_s))
				return (0);
		if (i >= ft_strlen(ptr))
			break;
	}
	return (1);
}
