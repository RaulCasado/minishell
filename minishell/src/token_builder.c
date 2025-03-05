
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

static int	split_input(ssize_t i, char *ptr, t_token **tokens)
{
	ssize_t j;
	ssize_t symbol_len;
	char *new_word;
	t_token *new_token;
	char quote;

	i = -1;
	while (ptr[++i])
	{
		symbol_len = ft_symbol_len(&ptr[i]);
		if (symbol_len > 0)
		{
			new_word = ft_substr(ptr, i, symbol_len);
			if (!new_word)
				return 0;
			add_token(tokens, new_word);
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
				return 0;
			}
			new_word = ft_substr(ptr, j, i - j + 1);
			if (!new_word)
				return 0;
			add_token(tokens, new_word);
		}
		else if (ptr[i] != ' ')
		{
			j = i;
			while (ptr[i + 1] != ' ' && ptr[i + 1] != '\0'
				&& !ft_symbol_len(&ptr[i + 1]) && ptr[i + 1] != '"' && ptr[i + 1] != '\'')
				i++;
			new_word = ft_substr(ptr, j, i - j + 1);
			if (!new_word)
				return (0);
			add_token(tokens, new_word);
		}
	}
	return 1;
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

t_token	*tokenize_input(char *input,t_minishell *minishell)
{
	t_token	*tokens;

	tokens = NULL;
	split_input(0, input, &tokens);
	expand_tokens(&tokens,minishell);
	if (!tokens || !tokenize_check(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	unquoter(&tokens);
	return (tokens);
}
