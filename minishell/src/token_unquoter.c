
#include "minishell.h"

static char	*unquote_token(t_token *token, size_t value_len)
{
	char	*new_value;

	if (value_len == 2)
	{
		free(token->value);
		return (NULL);
	}
	new_value = malloc(sizeof(char) * (value_len - 1));
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
			new_value = unquote_token(current, ft_strlen(current->value));
			if (new_value)
				current->value = new_value;
			else
			{
				if (!before)
					tokens = &current->next;
				else
					before->next = current->next;
				current = current->next;
				continue;
			}
		}
		before = current;
		current = current->next;
	}
}
