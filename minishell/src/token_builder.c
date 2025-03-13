#include "minishell.h"

static ssize_t	ft_symbol_len(char *ptr)
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

/* 1:
new_word = ft_substr(ptr, i, symbol_len);
if (!new_word)
	return (0); // Free words 0..i
add_token(tokens, new_word);
*/
/* 2:
new_word = ft_substr(ptr, j, i - j + 1);
if (!new_word)
	return (0); // Free words 0..i
add_token(tokens, new_word);
*/
/* 3:
new_word = ft_substr(ptr, j, i - j + 1);
if (!new_word)
	return (0); // Free words 0..i+j+h+23???
add_token(tokens, new_word);
*/

static int	build_and_add(char *ptr, size_t start, size_t len, t_token **tokens)
{
	char	*new_word;

	new_word = ft_substr(ptr, start, len);
	if (!new_word)
		return (0);
	add_token(tokens, new_word);
	return (1);
}

static int	split_input(ssize_t i, char *ptr, t_token **tokens)
{
	ssize_t	j;
	ssize_t	symbol_len;
	char	*new_word;
	t_token	*new_token;
	char	quote;

	i = -1;
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
			quote = ptr[i];
			j = i++;
			while (ptr[i] && ptr[i] != quote)
				i++;
			if (!ptr[i])
			{
				printf("Syntax error: unclosed quotes\n");
				return (0); // Free words ?
			}
			if (!build_and_add(ptr, j, i - j + 1, tokens))
				return (0); // Frees
		}
		else if (ptr[i] != ' ')
		{
			j = i;
			while (ptr[i + 1] != ' ' && ptr[i + 1] != '\0'
				&& !ft_symbol_len(&ptr[i + 1])
				&& ptr[i + 1] != '"' && ptr[i + 1] != '\'')
				i++;
			if (!build_and_add(ptr, j, i - j + 1, tokens))
				return (0); // Frees
		}
	}
	return (1);
}

/*
0: echo
1: -n
2: -n
3: -m
4: "hola que tal"
5: camello

(symbol) + cmd + (-n -m -l) + (arg) + (symbol / EOF) + cmd ...

i = 0;
if tokens[i] is symbol
	i++;

Loop:
	cmd/file = word[i]
	i++;

	param = NULL;
	if (cmd == echo)
		while (word[i] == '-n')
			param = "-n"
			i++;
	else if (cmd == pwd)
		fwojqpwfjpw
	
	arg = ""
	while (word[i] != symbol)
		arg += " " + word[i]
		i++;
	
echo -n -n -m "'$USER'" caballo
(echo) (-n) (-m "'$USER'" caballo)
*/

t_token	*tokenize_input(char *input, t_minishell *minishell)
{
	t_token	*tokens;

	tokens = NULL;
	split_input(0, input, &tokens);
	free(input);
	expand_tokens(&tokens, minishell);
	if (!tokens || !tokenize_check(tokens))
	{
		free_tokens(tokens); // if !tokens why free?
		return (NULL);
	}
	unquoter(&tokens);
	return (tokens);
}
