/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_extras.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: droura-s <droura-s@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:06:08 by droura-s          #+#    #+#             */
/*   Updated: 2025/03/10 14:06:32 by droura-s         ###   ########.fr       */
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
	ssize_t	i;

	i = 0;
	while (i < len)
		free(ptr[i++]);
	free(ptr);
}
