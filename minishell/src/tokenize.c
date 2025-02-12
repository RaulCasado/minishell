
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

static void	split_input(ssize_t	i, char *ptr, t_token **tokens)
{
	ssize_t	j;
	ssize_t	symbol_len;
	char	*new_word;
	t_token	*new_token;

	i = -1;
	while (ptr[++i])
	{
		symbol_len = ft_symbol_len(&ptr[i]);
		if (symbol_len > 0)
		{
			new_word = ft_substr(ptr, i, symbol_len);
			add_token(tokens, new_word);
			i += symbol_len - 1;
		}
		else if (ptr[i] != ' ')
		{
			j = i;
			while (ptr[i + 1] != ' ' && ptr[i + 1] != '\0'
				&& !ft_symbol_len(&ptr[i + 1]))
				i++;
			new_word = ft_substr(ptr, j, i - j + 1);
			add_token(tokens, new_word);
		}
	}
}

t_token	*tokenize_input(char *input)
{
	t_token	*tokens;
	size_t	count;

	tokens = NULL;
	split_input(0, input, &tokens);
	//join_tokens(&tokens); // Check Marks
	if (!tokenize_check(tokens))
	{
		ft_putendl_fd("ERROR in input commands", 2);
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}
