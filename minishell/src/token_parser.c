/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:33:42 by racasado          #+#    #+#             */
/*   Updated: 2025/03/28 11:39:24 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	CHECK combinations

	<infile ls -l | wc -l > outfile
	ls -l > outfile
	ls -l >			::: Error
	ls -l <

	ls | >infile
	ls | 		::: Error
	ls | <		::: Error
	echo hola || echo mundo 
	// this i a strange case xd i tried
	echo hola || echo mundo this and should work but
	echo hola | | echo mundo should not
	// i tried it in other minishell and both should fail
	tested sytnax from minishell tester
	echo hi | "|" its ok but should fail doesnt make sense lol
	check if its correct pipe at the start or end of command
*/
static int	has_open_quotes(char *str)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			if (!quote) // Si no hay comilla abierta, la abrimos
				quote = str[i];
			else if (quote == str[i])
				quote = 0; // Si cerramos la misma comilla, la reiniciamos
		}
		i++;
	}
	return (quote != 0);
}

int	is_invalid_character_outside_quotes(char *str)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			if (!quote)
				quote = str[i];
			else if (quote == str[i])
				quote = 0;
		}
		else if (!quote && (str[i] == '\\' || str[i] == ';' || str[i] == '&'))
			return (1);
		i++;
	}
	return (0);
}

static int	check_syntax(t_token *before, t_token *current)
{
	//ERROR: Redirección sin archivo después
	if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT
		|| current->type == TOKEN_REDIR_APPEND
		|| current->type == TOKEN_HEREDOC)
	{
		if (!(current->next && current->next->type == TOKEN_WORD))
			return (printf("Syntax error: unexpected token after '%s'\n",
					current->value));
	}
	//ERROR: Pipes incorrectos
	else if (current->type == TOKEN_PIPE)
	{
		if (!before || !current->next || current->next->type == TOKEN_PIPE)
			return (printf("Syntax error: unexpected token '%s'\n",
					current->value));
	}
	//ERROR: `||` no permitido
	else if (before && before->type == TOKEN_PIPE
		&& current->type == TOKEN_PIPE)
		return (printf("Syntax error: unexpected token '||'\n"));
	return (0);
}

static int	check_words(t_token *current)
{
	//ERROR: Caracteres especiales no permitidos
	if (current->type == TOKEN_WORD
		&& is_invalid_character_outside_quotes(current->value))
		return (printf("Syntax error: invalid character in input: '%s'\n",
				current->value));
	//ERROR: Comillas abiertas
	else if (current->type == TOKEN_WORD && has_open_quotes(current->value))
		return (printf("Syntax error: unclosed quotes in '%s'\n",
				current->value));
	//ERROR: Token dentro de comillas que contiene solo `|`
	else if (current->type == TOKEN_WORD && ft_strlen(current->value) == 3
		&& current->value[0] == '"' && current->value[1] == '|'
		&& current->value[2] == '"')
	{
		return (printf("Syntax error: invalid token inside quotes '%s'\n",
				current->value));
	}
	return (0);
}

int	tokenize_check(t_token *tokens)
{
	t_token	*before;
	t_token	*current;

	before = NULL;
	current = tokens;
	while (current)
	{
		if (check_syntax(before, current) || check_words(current))
			return (0);
		before = current;
		current = current->next;
	}
	return (1);
}
