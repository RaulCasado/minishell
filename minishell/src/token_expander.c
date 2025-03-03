
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

static char	*expand(char *value, ssize_t start, ssize_t end, size_t	len, t_minishell *minishell)
{
	char	*path;
	char	*head;
	char	*body;
	char	*tail;
	char	*new_value;

	path = ft_substr(value, start, end - start + 1);
	if (!path)
		exit(1); // TODO :: Malloc error
	head = ft_substr(value, 1, start - 2);
	if (!head)
		exit(1); // TODO :: Malloc error
	body = get_env(path, minishell->envp);
	/* free(path); */
	if (!body)
		return (ft_strdup("")); // TODO :: Path not found
	if (end + 1 >= ft_strlen(value) - 1)
		tail = NULL;
	else
	{
		tail = ft_substr(value, end + 1, ft_strlen(value) - end - 2);
		if (!tail)
			exit(1); // TODO :: Malloc error
	}
	new_value = forge_value(head, body, tail);
	if (!new_value)
		exit(1); // TODO :: Calloc error
	/* free(value); */
	return (new_value);		
}


static char	*expand_exit_code(char *value, ssize_t start, ssize_t end, size_t len, t_minishell *minishell)
{
	char	*new_value;

	new_value = ft_itoa(minishell->exit_code);
	if (!new_value)
		exit(1); // TODO :: Calloc error
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
CUIDADADO TAMBIEN TENEMOS QUE TENER EN CUENTA LO DE $? QUE ES EL ULTIMO STATUS DE SALIDA
*/

// TODO
/*
echo " '$USER' "
echo "$USER" "$-HOLA" "$1ERROR" "$ ERROR" $ERROR
*/
void	expand_tokens(t_token **tokens,t_minishell *minishell)
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
				if (current->value[i] == DOLLAR && (current->value[i + 1] == HYPHEN
					|| ft_isalnum(current->value[i + 1])
					|| current->value[i + 1] == INTERROGATION))
				{
					start = ++i;
					if (current->value[i] == INTERROGATION) // This logic is working but it's badly coded, it needs simplicity
					{
						current->value = expand_exit_code(current->value, start, i - 1, ft_strlen(current->value), minishell);
						start = 0;
					}
				}
				if (start && !ft_isalnum(current->value[i]) && current->value[i] != '_')
				{
					current->value = expand(current->value, start, i - 1, ft_strlen(current->value), minishell);
					start = 0;
				}
			}
		}
		current = current->next;
	}
}
