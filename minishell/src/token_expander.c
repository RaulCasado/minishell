/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: droura-s <droura-s@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:33:35 by racasado          #+#    #+#             */
/*   Updated: 2025/03/19 13:51:04 by droura-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	if (token->value[*i] && token->value[*i] != ' ')
		*start = *i;
	else if (*start != -1 && (!ft_isalnum(token->value[*i]) && token->value[*i] != '_'))
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
*/

static int	expand_loop(t_token *token, t_minishell *minishell,
	ssize_t *i)
{
	// printf("\t%c\n", token->value[*i]);
	(*i)++;
	if (!token->value[*i] || token->value[*i] == ' ')
		return (0);

	ssize_t	j = *i + 1;
	while (token->value[j + 1] && token->value[j + 1] != ' ' && token->value[j + 1] != DOUBLE_MARK)
		j++;

	token->value = expand_variable(token->value, (*i), j, minishell);
	// (*i) = -1;
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
	ssize_t	start;
	char	marks;

	marks = 0;
	current = *tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			i = 0;
			while (current->value[i])
			{
				if (current->value[i] == DOUBLE_MARK)
				{
					if (marks)
						marks = 0;
					else
						marks = 1;
				}
				else if (marks && current->value[i] == DOLLAR && closed_quotes(current->value + i))
					if (!expand_loop(current, minishell, &i))
						break ;
				i++;
			}
		}
		current = current->next;
	}
}

/*
echo $USER "$USER" '$USER'

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
