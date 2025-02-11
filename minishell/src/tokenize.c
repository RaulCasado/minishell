
#include "minishell.h"

static size_t	is_symbol(char *ptr)
{
	if (*ptr == '|')
		return (1);
	if (*ptr == '>')
	{
		if (*(ptr++) == '>')
			return (2);
		return (1);
	}
	else if (*ptr == '<')
	{
		if (*(ptr++) == '<')
			return (2);
		return (1);
	}
	return (0);
}

static size_t	count_tokens(char *ptr)
{
	size_t	total_words;
	size_t	i;
	size_t	symbol;

	i = 0;
	total_words = 0;
	while (ptr[i])
	{
		symbol = is_symbol(&ptr[i]);
		if (ptr[i] == ' ')
		{
			total_words++;
			while (ptr[i] && ptr[i] != ' ')
				i++;
		}
		else if (symbol)
		{
			total_words++;
			if (symbol == 2)
				i++;
		}
		else
			i++;
	}
	return (total_words);
}

t_token	*tokenize_input(char *input)
{
	t_token	*tokens;

	(void) input;
	(void) tokens;
	return (NULL);
}
