/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:32:59 by racasado          #+#    #+#             */
/*   Updated: 2025/03/14 17:33:00 by racasado         ###   ########.fr       */
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
