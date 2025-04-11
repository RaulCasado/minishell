/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_builder_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 18:54:11 by droura-s          #+#    #+#             */
/*   Updated: 2025/04/11 12:45:52 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	build_and_add(char *ptr, size_t s, size_t l, t_token **t)
{
	char	*new_word;

	new_word = ft_substr(ptr, s, l);
	if (!new_word)
	{
		free_tokens(*t);
		*t = NULL;
		return (0);
	}
	if (ft_strlen(new_word))
		add_token(t, new_word);
	else
		free(new_word);
	return (1);
}

static void	handle_marks_struct(char c, t_marks *m)
{
	if (c == '"' && !m->s)
		m->d = !m->d;
	else if (c == '\'' && !m->d)
		m->s = !m->s;
}

static int	handle_word(char *p, ssize_t *i, t_token **t, t_marks *m)
{
	ssize_t	s;

	s = *i;
	while (p[*i])
	{
		if (!(m->d || m->s) && ft_symbol_len(&p[*i]) > 0)
			break ;
		handle_marks_struct(p[*i], m);
		if ((p[*i] == ' ' || p[*i] == '\t'
				|| p[*i] == '\n') && !m->d && !m->s)
			break ;
		(*i)++;
	}
	return (build_and_add(p, s, *i - s, t));
}

int	split_input(ssize_t i, char *p, t_token **t)
{
	ssize_t	len;
	t_marks	m;

	m.d = 0;
	m.s = 0;
	i = 0;
	while (p[i])
	{
		while ((p[i] == ' ' || p[i] == '\n'
				|| p[i] == '\t') && !m.d && !m.s)
			i++;
		if (!p[i])
			break ;
		len = ft_symbol_len(&p[i]);
		if (len > 0 && !m.d && !m.s)
		{
			if (!build_and_add(p, i, len, t))
				return (0);
			i += len;
			continue ;
		}
		if (!handle_word(p, &i, t, &m))
			return (0);
	}
	return (1);
}
