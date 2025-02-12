
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

int	tokenize_check(t_token *tokens)
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
