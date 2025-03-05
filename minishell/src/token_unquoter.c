
#include "minishell.h"

static char	*unquote_token(t_token *token, size_t value_len)
{
	char	*new_value;

	if (value_len == 2)
	{
		free(token->value);
		return (ft_strdup(""));
	}
	new_value = malloc(sizeof(char) * (value_len - 1));
	if (!new_value)
	{
		free(token->value);
		return (NULL);
	}
	ft_memcpy(new_value, token->value + 1, value_len - 2);
	free(token->value);
	return (new_value);
}

void	unquoter(t_token **tokens)
{
	t_token	*before;
	t_token	*current;
	char	*new_value;

	before = NULL;
	current = *tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD
			&&(current->value[0] == DOUBLE_MARK
			|| current->value[0] == SIMPLE_MARK))
		{
			current->value = unquote_token(current, ft_strlen(current->value));
			if (!current->value)
				exit(3); // TODO Error in ft_strdup or malloc
		}
		before = current;
		current = current->next;
	}
}
