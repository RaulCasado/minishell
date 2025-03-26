/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expander_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: droura-s <droura-s@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:33:26 by racasado          #+#    #+#             */
/*   Updated: 2025/03/26 14:07:45 by droura-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*free_mallocs(char *head, char *expansion, char *tail, char *value)
{
	if (expansion && !expansion[0])
		free(expansion);
	if (head)
		free(head);
	if (tail)
		free(tail);
	if (value)
		free(value);
	return (NULL);
}

static char	*get_variable(char *value, ssize_t start,
	ssize_t end, t_minishell *minishell)
{
	char	*expansion;
	char	*var_name;

	if (start == end && value[start] == QUESTION_MARK)
		expansion = ft_itoa(minishell->exit_code);
	else
	{
		var_name = ft_substr(value, start, end - start + 1);
		if (!var_name)
			return (NULL);
		expansion = get_env(var_name, minishell->envp);
		free(var_name);
	}
	if (!expansion)
		expansion = ft_strdup("");
	return (expansion);
}

static char	*forge_value(char *head, char *expansion, char *tail)
{
	char	*new_value;
	size_t	len_head;
	size_t	len_expansion;
	size_t	len_tail;

	len_head = ft_strlen(head);
	len_expansion = ft_strlen(expansion);
	if (!tail)
		len_tail = 0;
	else
		len_tail = ft_strlen(tail);
	new_value = ft_calloc(len_head + len_expansion + len_tail + 1,
			sizeof(char));
	if (!new_value)
		return (NULL);
	ft_memcpy(new_value, head, len_head);
	ft_memcpy(new_value + len_head, expansion, len_expansion);
	ft_memcpy(new_value + len_head + len_expansion, tail, len_tail);
	new_value[len_head + len_expansion + len_tail] = '\0';
	return (new_value);
}

char	*expand_variable(char *value, ssize_t start,
	ssize_t end, t_minishell *minishell)
{
	char	*expansion;
	char	*head;
	char	*tail;
	char	*new_value;

	expansion = get_variable(value, start, end, minishell);
	if (!expansion)
		return (NULL);
	head = ft_substr(value, 0, start - 1);
	if (!head)
		return (free_mallocs(NULL, expansion, NULL, NULL));
	if (value[end] == DOUBLE_MARK)
		tail = ft_strdup("\"");
	else
		tail = ft_substr(value, end + 1, ft_strlen(value) - end);
	if (!tail)
		return (free_mallocs(head, expansion, NULL, NULL));
	new_value = forge_value(head, expansion, tail);
	if (!new_value)
		return (free_mallocs(head, expansion, tail, NULL));
	free_mallocs(head, expansion, tail, value);
	return (new_value);
}
