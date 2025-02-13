
#include "minishell.h"

static char	*expand(char *value, ssize_t start, ssize_t end, size_t	len)
{
	char	*expansion;
	char	*head;
	char	*body;
	char	*tail;

	expansion = ft_substr(value, start, end - start + 1);
	// TODO IF (!head)
	head = "";// ft_substr(value, 1, len - ft_strlen());
	body = getenv(expansion);
	tail = "";//ft_substr(value, start, );
	printf("\t\t%s%s%s // %s\n", head, body, tail, expansion);
	/*
	Minishell> echo "$USER" "$-HOLA" "$1ERROR" "$ ERROR" $ERROR
                droura-s // USER
                (null) //
	zsh: segmentation fault (core dumped)  ./minishell
	*/
	return (ft_strjoin(ft_strjoin(head, body), tail));
}

// TODO
/*
echo " '$USER' "
echo "$USER" "$-HOLA" "$1ERROR" "$ ERROR" $ERROR
*/
void	expand_tokens(t_token **tokens)
{
	t_token	*current;
	ssize_t	i;
	ssize_t	start;
	char	chr;

	current = *tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD && current->value[0] == DOUBLE_MARK
			&& current->value[ft_strlen(current->value) - 1] == DOUBLE_MARK)
		{
			i = -1;
			start = 0;
			while (current->value[++i])
			{
				if (current->value[i] == DOLLAR && (current->value[i + 1] == '-'
					|| ft_isalpha(current->value[i + 1])))
					start = ++i;
				if (start && !ft_isalnum(current->value[i])) // _ - etc
				{
					current->value = expand(current->value, start, i - 1, ft_strlen(current->value));
					start = 0;
				}
			}
		}
		current = current->next;
	}
}
