
#include "minishell.h"

t_token	*create_token(char *value)
{
	//value is a string allocated by malloc
	t_token	*token;

	if (!value)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (ft_strncmp(value, "|", 1) == 0)
		token->type = TOKEN_PIPE;
	else if (ft_strncmp(value, "<", 1) == 0)
		token->type = TOKEN_REDIR_IN;
	else if (ft_strncmp(value, ">", 1) == 0)
		token->type = TOKEN_REDIR_OUT;
	else if (ft_strncmp(value, ">>", 2) == 0)
		token->type = TOKEN_REDIR_APPEND;
	else if (ft_strncmp(value, "<<", 2) == 0)
		token->type = TOKEN_HEREDOC;
	else
		token->type = TOKEN_WORD;
	token->value = value;
	token->next = NULL;
	return (token);
}
void add_token(t_token **tokens, char *value)
{
	t_token	*last;
	t_token	*new_token;

	new_token = create_token(value);
	if (!new_token)
		return ;
	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	last = *tokens;
	while (last->next)
		last = last->next;
	last->next = new_token;
}
