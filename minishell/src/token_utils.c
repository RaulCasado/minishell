/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:34:00 by racasado          #+#    #+#             */
/*   Updated: 2025/03/14 17:34:01 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_token_len(t_token *token)
{
	t_token	*current;
	size_t	len;

	len = 0;
	current = token;
	while (current && ++len)
		current = current->next;
	return (len);
}

t_token	*ft_get_last(t_token *token)
{
	t_token	*current;

	if (!token)
		return (NULL);
	current = token;
	while (current->next)
		current = current->next;
	return (current);
}

t_token	*ft_get_token(t_token *token, size_t i)
{
	t_token	*current;

	if (!token || i >= ft_token_len(token))
		return (NULL);
	current = token;
	while (i--)
		current = current->next;
	return (current);
}

// TODO // Check if it's working
char	ft_add_token(t_token **token, t_token *new_token, size_t i)
{
	t_token	*before;
	t_token	*current;

	if (!token || i >= ft_token_len(*token))
		return (1);
	before = NULL;
	current = *token;
	while (i--)
	{
		before = current;
		current = current->next;
	}
	new_token->next = current;
	if (before)
		before->next = new_token;
	else
		*token = new_token;
	return (0);
}

// TODO // Check if it's working
char	ft_remove_token(t_token **token, size_t i)
{
	t_token	*before;
	t_token	*current;

	if (!token || i >= ft_token_len(*token))
		return (1);
	before = NULL;
	current = *token;
	while (i--)
	{
		before = current;
		current = current->next;
	}
	if (before)
		before->next = current->next;
	else
		*token = current->next;
	free(current->value);
	free(current);
	return (0);
}
