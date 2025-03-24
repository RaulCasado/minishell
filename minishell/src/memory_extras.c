/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_extras.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:06:08 by droura-s          #+#    #+#             */
/*   Updated: 2025/03/15 21:25:40 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_envp(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}

void	free_ptr_ptr(char **ptr, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
		free(ptr[i++]);
	free(ptr);
}

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}
