/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 17:30:48 by racasado          #+#    #+#             */
/*   Updated: 2025/02/11 19:16:36 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int g_signal = 1;

int get_signal()
{
	return (g_signal);
}
void set_signal(int signal)
{
	g_signal = signal;
}

//for ctrl+c shouldnt do anything
void handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

//for ctrl+d should exit
void handle_eof()
{
	printf("Saliendo de Minishell...\n");
	set_signal(0);
}

//for ctrl+\ shouldnt do anything this is so bugged
void handle_sigquit(int sig)
{
	(void)sig;
}

void	setup_signals(void)
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	// Configurar Ctrl+C (SIGINT)
	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = SA_RESTART;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);

	// Configurar Ctrl+\ (SIGQUIT) -> No hace nada
	sa_quit.sa_handler = handle_sigquit;
	sa_quit.sa_flags = SA_RESTART;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}