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

static char	*expand(char *value, ssize_t start,
	ssize_t end, t_minishell *minishell)
{
	char	*path;
	char	*head;
	char	*body;
	char	*tail;
	char	*new_value;

	path = ft_substr(value, start, end - start + 1);
	if (!path)
		exit(1); // TODO :: Malloc error
	head = ft_substr(value, 0, start - 1);
	if (!head)
		exit(1); // TODO :: Malloc error
	body = get_env(path, minishell->envp);
	/* free(path); */
	if (!body) // TODO :: Path not found // ERRO RANDOM SYMBOLS
		return (ft_strdup(DOUBLE_DOUBLE_MARK_STR));
	if (end + 1 >= ft_strlen(value) - 1)
		tail = DOUBLE_MARK_STR;
	else
	{
		tail = ft_substr(value, end + 1, ft_strlen(value) - end - 1);
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

// echo hola "$USER $?" adios
void	expand_tokens(t_token **tokens, t_minishell *minishell)
{
	t_token	*current;
	ssize_t	i, start;

	current = *tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			i = 0;
			start = -1;
			while (current->value[i])
			{
				if (current->value[i] == DOLLAR && (ft_isalpha(current->value[i + 1]) ||
					current->value[i + 1] == '_' || current->value[i + 1] == QUESTION_MARK))
				{
					start = ++i;
					if (current->value[i] == QUESTION_MARK)
					{
						current->value = expand_exit_code(current->value, start, i, ft_strlen(current->value), minishell);
						i = 0; // Reiniciar para procesar correctamente
						start = -1;
						continue;
					}
				}
				else if (start != -1 && (!ft_isalnum(current->value[i]) && current->value[i] != '_'))
				{
					current->value = expand(current->value, start, i - 1, minishell);
					i = 0; // Reiniciar para analizar de nuevo desde el principio
					start = -1;
					continue;
				}
				i++;
			}
		}
		current = current->next;
	}
}

void	expand_tokens2(t_token **tokens, t_minishell *minishell)
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
			start = -1;
			
		}
		current = current->next;
	}
}

/*
while (current->value[++i])
			{
				
			}
			
if (current->value[i] == DOLLAR && (current->value[i + 1] == HYPHEN
	|| ft_isalnum(current->value[i + 1])
	|| current->value[i + 1] == QUESTION_MARK))
{
	start = ++i;
	if (current->value[i] == QUESTION_MARK)
	{
		current->value = expand_exit_code(current->value, start, i - 1, ft_strlen(current->value), minishell);
		start = 0;
	}
}
if (start && !ft_isalnum(current->value[i]) && current->value[i] != '_')
{
	current->value = expand(current->value, start, i - 1, minishell);
	start = 0;
}
*/
