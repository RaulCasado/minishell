/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:33:42 by racasado          #+#    #+#             */
/*   Updated: 2025/04/07 20:09:38 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			if (!quote)
				quote = str[i];
			else if (quote == str[i])
				quote = 0;
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
	if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT
		|| current->type == TOKEN_REDIR_APPEND
		|| current->type == TOKEN_HEREDOC)
	{
		if (!(current->next && current->next->type == TOKEN_WORD))
			return (printf("Syntax error: unexpected token after '%s'\n",
					current->value));
	}
	else if (current->type == TOKEN_PIPE)
	{
		if (!before || !current->next || current->next->type == TOKEN_PIPE)
			return (printf("Syntax error: unexpected token '%s'\n",
					current->value));
	}
	else if (before && before->type == TOKEN_PIPE
		&& current->type == TOKEN_PIPE)
		return (printf("Syntax error: unexpected token '||'\n"));
	return (0);
}

static int	check_words(t_token *current)
{
	if (current->type == TOKEN_WORD
		&& is_invalid_character_outside_quotes(current->value))
		return (printf("Syntax error: invalid character in input: '%s'\n",
				current->value));
	else if (current->type == TOKEN_WORD && has_open_quotes(current->value))
		return (printf("Syntax error: unclosed quotes in '%s'\n",
				current->value));
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
