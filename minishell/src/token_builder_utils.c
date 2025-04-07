/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_builder_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 18:54:11 by droura-s          #+#    #+#             */
/*   Updated: 2025/04/07 11:29:59 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int build_and_add(char *ptr, size_t start, size_t len, t_token **tokens)
{
	char *new_word;

	new_word = ft_substr(ptr, start, len);
	if (!new_word) // ????
	{
		printf("FREEEEEEEING\n\n");
		free_tokens(*tokens);
		*tokens = NULL;
		return (0);
	}
	if (!ft_strlen(new_word))
		free(new_word);
	else
		add_token(tokens, new_word);
	return (1);
}

static int handle_word(char *ptr, ssize_t *i, t_token **tokens, char *mark_d, char *mark_s)
{
    ssize_t start = *i;
    while (ptr[*i])
    {
        // Si no estamos dentro de comillas y encontramos un símbolo especial, salimos.
        if (!(*mark_d || *mark_s) && ft_symbol_len(&ptr[*i]) > 0)
            break;
        // Actualizamos el estado de las comillas.
        handle_marks(ptr[*i], mark_d, mark_s);
        // Si encontramos un espacio fuera de comillas, cortamos el token.
        if (ptr[*i] == ' ' && !(*mark_d || *mark_s))
            break;
        (*i)++;
    }
    return (build_and_add(ptr, start, *i - start, tokens));
}

int split_input(ssize_t i, char *ptr, t_token **tokens)
{
    ssize_t symbol_len;
    char mark_d = 0;
    char mark_s = 0;

    i = 0;
    while (ptr[i])
    {
        // Salta espacios fuera de comillas.
        if (ptr[i] == ' ' && !mark_d && !mark_s)
        {
            i++;
            continue;
        }
        // Si se detecta un símbolo especial (como <, >, |, etc.) fuera de comillas,
        // se crea un token con ese símbolo.
        symbol_len = ft_symbol_len(&ptr[i]);
        if (symbol_len > 0 && !mark_d && !mark_s)
        {
            if (!build_and_add(ptr, i, symbol_len, tokens))
                return (0);
            i += symbol_len;
            continue;
        }
        // Procesa una palabra (que puede incluir comillas o caracteres alfanuméricos).
        if (!handle_word(ptr, &i, tokens, &mark_d, &mark_s))
            return (0);
    }
    return (1);
}
