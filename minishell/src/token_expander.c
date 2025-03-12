#include "minishell.h"

static char	*forge_value(char *head, char *body, char *tail)
{
	char	*new_value;
	char	len_head;
	char	len_body;
	char	len_tail;

	len_head = ft_strlen(head);
	len_body = ft_strlen(body);
	if (!tail)
		len_tail = 0;
	else
		len_tail = ft_strlen(tail);
	new_value = ft_calloc(len_head + len_body + len_tail + 1, sizeof(char));
	if (!new_value)
		return (NULL);
	ft_memcpy(new_value, head, len_head);
	ft_memcpy(new_value + len_head, body, len_body);
	ft_memcpy(new_value + len_head + len_body, tail, len_tail);
	new_value[len_head + len_body + len_tail + 1] = '\0';
	/* free(head);
	free(body);
	if (tail)
		free(tail); */
	return (new_value);
}

static char	*expand_variable(char *value, ssize_t start, ssize_t end, t_minishell *minishell)
{
	char	*expansion;
	char	*head;
	char	*tail;
	char	*new_value;
	char	*var_name;

	if (end == -1)
	{
		expansion = ft_itoa(minishell->exit_code);
		end = start;
	}
	else
	{
		var_name = ft_substr(value, start, end - start + 1);
		if (!var_name)
			exit(1); // TODO: Malloc error
		expansion = get_env(var_name, minishell->envp);
		/* free(var_name); */
		if (!expansion) // Si no existe, se devuelve una cadena vacía
			expansion = ft_strdup("");
	}
	head = ft_substr(value, 0, start - 1);
	if (!head)
		exit(1); // TODO: Malloc error
	/* FIX THIS LINE ==> x ? y : z   NO.*/
	tail = (end + 1 >= ft_strlen(value)) ? ft_strdup("") : ft_substr(value, end + 1, ft_strlen(value) - end - 1);
	if (!tail)
		exit(1); // TODO: Malloc error
	new_value = forge_value(head, expansion, tail);
	if (!new_value)
		exit(1); // TODO: Calloc error
	/* free(value); */
	return (new_value);
}


/*
	echo "$USER" "$-HOLA" "$1ERROR" "$ ERROR" $ERROR
droura-s 569XZimsHOLA ERROR $ ERROR
	echo "$USER" "$-HOLA"                           
droura-s 569XZimsHOLA
	echo "$USER" "$-"    
droura-s 569XZims en el de prueba sale droura-s $-

echo " '$USER' "
echo "$USER" "$-HOLA" "$1ERROR" "$ ERROR" $ERROR
echo hola "$USER $?" adios
*/
void	expand_tokens(t_token **tokens, t_minishell *minishell)
{
	t_token	*current;
	ssize_t	i;
	ssize_t	start;

	current = *tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			i = -1;
			start = -1;
			while (current->value[++i])
			{
				if (current->value[i] == DOLLAR && (ft_isalpha(current->value[i + 1]) ||
					current->value[i + 1] == '_' || current->value[i + 1] == QUESTION_MARK))
				{
					start = ++i;
					if (current->value[i] == QUESTION_MARK)
					{
						current->value = expand_variable(current->value, start, -1, minishell);
						start = -1;
					}
				}
				else if (start != -1 && (!ft_isalnum(current->value[i]) && current->value[i] != '_'))
				{
					current->value = expand_variable(current->value, start, i - 1, minishell);
					start = -1;
				}
			}
		}
		current = current->next;
	}
}
