/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 22:05:14 by racasado          #+#    #+#             */
/*   Updated: 2025/04/08 14:01:35 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_main(t_minishell **minishell, char **envp)
{
	setup_signals();
	rl_catch_signals = 0;
	/* if (isatty(STDIN_FILENO) == 0)
		dup2(open("/dev/tty", O_RDONLY), STDIN_FILENO); */
	*minishell = minishell_builder(envp);
	if (!(*minishell))
	{
		ft_putendl_fd("Error: Failed to initialize minishell", 2);
		exit(1);
	}
}
