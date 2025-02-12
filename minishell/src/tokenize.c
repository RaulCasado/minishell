
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
	// this i a strange case xd i tried echo hola || echo mundo this and should work but echo hola | | echo mundo should not
	// i tried it in other minishell and both should fail
	tested sytnax from minishell tester
	echo hi | "|" its ok but should fail doesnt make sense lol
	check if its correct pipe at the start or end of command
*/

static int has_open_quotes(char *str)
{
	int i;
	int	j;

	//possible issue echo "'hola"'
	i = 0;
	j = ft_strlen(str) - 1;
	while (i < j && (str[i] == '\'' || str[j] == '\''
		|| str[i] == '\"' || str[j] == '\"'))
	{
		if (str[i] != str[j])
			return (1);
		i++;
		j--;
	}
	return (0);
}

static int	tokenize_check(t_token *tokens)
{
	t_token	*before;
	t_token	*current;

	before = NULL;
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT
		|| current->type == TOKEN_REDIR_APPEND || current->type == TOKEN_HEREDOC)
		{	
			if (!(current->next && current->next->type == TOKEN_WORD))
				return (printf("Syntax error: unexpected token after '%s'\n", current->value), 0);
		}
		else if (current->type == TOKEN_PIPE)
		{
			if (!before || !current->next || current->next->type == TOKEN_PIPE)
				return (printf("Syntax error: unexpected token after '%s'\n", current->value), 0);
		}
		else if (current->type == TOKEN_WORD)
		{
			if (has_open_quotes(current->value))
				return (printf("Syntax error: open quotes in '%s'\n", current->value), 0);
		}
		before = current;
		current = current->next;
	}
	return (1);
}

static void	join_tokens(t_token *tokens)
{
	(void) tokens;
}

t_token	*tokenize_input(char *input)
{
	t_token	*tokens;
	size_t	count;

	tokens = NULL;
	split_input(0, input, &tokens);
	join_tokens(tokens);
	if (!tokenize_check(tokens))
	{
		ft_putendl_fd("ERROR in input commands", 2);
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}
