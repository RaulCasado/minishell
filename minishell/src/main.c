/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:20:26 by racasado          #+#    #+#             */
/*   Updated: 2025/02/17 16:38:02 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// main principal del minichell

#include "minishell.h"

void minishell_loop() {
	char	*input;
	t_token	*tokens;

	setup_signals();
	rl_catch_signals = 0;

	while (get_signal()) {
		input = readline("Minishell> ");
		tokens = NULL;

		if (!input) {
			handle_eof();
			break;
		}
		if (*input == '\0') {
			free(input);
			continue;
		}

		add_history(input);
		// If the command is exi it also count :c
		if (ft_strlen(input) == 4 && ft_strncmp(input, "exit", 5) == 0)
		{
			free(input);
			printf("Saliendo de Minishell...\n");
			set_signal(0);
			break;
		}

		tokens = tokenize_input(input);
		print_tokens(tokens);
		if (!tokens)
		{
			(void) 0; // Execute command
			free_tokens(tokens);
			tokens = NULL;
		}

		printf("Comando ingresado: %s\n", input);

		free(input);
	}
}


int	main(int argc, char **argv)
{
	(void)argv; // i dont know if we will need this in the future
	if (argc > 1)
	{
		ft_putendl_fd("Error: Minishell no acepta argumentos", 2);
		return (1);
	}
	minishell_loop();
	return (0);
}