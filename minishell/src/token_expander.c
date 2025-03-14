/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:33:35 by racasado          #+#    #+#             */
/*   Updated: 2025/03/14 17:33:36 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_loop(t_token *token, t_minishell *minishell,
	size_t *i, size_t *start)
{
	if (token->value[*i] == DOLLAR && token->value[*i + 1]
		&& token->value[*i + 1] != ' ')
		*start = ++(*i);
	else if (*start != -1 && (!ft_isalnum(token->value[*i])
			&& token->value[*i] != '_'))
	{
		token->value = expand_variable(token->value, *start,
				(*i) - 1, minishell);
		*start = -1;
		if (token->value)
			*i = 0;
	}
	if (!token->value)
		return (0);
	return (1);
}

void	expand_tokens(t_token **tokens, t_minishell *minishell)
{
	t_token	*current;
	ssize_t	i;
	ssize_t	start;

	current = *tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			i = 0;
			start = -1;
			while (current->value[i])
			{
				if (!expand_loop(current, minishell, &i, &start))
					break ;
				i++;
			}
		}
		current = current->next;
	}
}

/*
	echo "$USER" "$-HOLA" "$1ERROR" "$ ERROR" $ERROR
droura-s 569XZimsHOLA ERROR $ ERROR
	echo "$USER" "$-HOLA"
droura-s 569XZimsHOLA
	echo "$USER" "$-"
droura-s 569XZims en el de prueba sale droura-s $-

echo " '$USER' "
echo "$USER" "$-HOLA" "$1ERROR" "$ ERROR" $ERROR
echo hola "$USER $?" adios
*/
