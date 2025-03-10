/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_global.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: droura-s <droura-s@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 17:30:48 by racasado          #+#    #+#             */
/*   Updated: 2025/03/10 14:34:48 by droura-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	g_signal = 1;

int	get_signal(void)
{
	return (g_signal);
}

void	set_signal(int signal)
{
	g_signal = signal;
}
