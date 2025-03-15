/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:32:59 by racasado          #+#    #+#             */
/*   Updated: 2025/03/15 21:25:00 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	write_strs(char **strs, char nl, char nll, int fd)
{
	char	*sep;

	sep = " ";
	if (nll)
		sep = "\n";
	while (*strs)
	{
		if (write(fd, *strs, ft_strlen(*strs)) == -1)
			return (-1);
		strs++;
		if (*strs && write(fd, sep, 1) == -1)
			return (-1);
	}
	if (nl && write(fd, "\n", 1) == -1)
		return (-1);
	return (0);
}

int	write_str(char *str, int fd)
{
	if (write(fd, str, ft_strlen(str)) == -1)
		return (-1);
	if (write(fd, "\n", 1) == -1)
		return (-1);
	return (0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, ECHO_LOWER) == 0
		|| ft_strcmp(cmd, CD_LOWER) == 0
		|| ft_strcmp(cmd, PWD_LOWER) == 0
		|| ft_strcmp(cmd, EXPORT_LOWER) == 0
		|| ft_strcmp(cmd, UNSET_LOWER) == 0
		|| ft_strcmp(cmd, ENV_LOWER) == 0
		|| ft_strcmp(cmd, EXIT_LOWER) == 0);
}
