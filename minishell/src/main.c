/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: droura-s <droura-s@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:20:26 by racasado          #+#    #+#             */
/*   Updated: 2025/02/11 16:45:45 by droura-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// main principal del minichell

#include "minishell.h"

void minishell_loop() {
	char	*input;
	t_token	*tokens;

	while (1) {
		input = readline("Minishell> ");
		tokens = NULL;
		if (!input || *input == 0)
			continue;
		if (*input)
			add_history(input);
		if (ft_strncmp(input, "exit", ft_strlen(input)) == 0)
		{
			free(input);
			printf("Saliendo de Minishell...\n");
			exit(0);
		}
		
		// Tenemos que ver cómo hacer la cadena de sentencias IF
		tokens = tokenize_input(input);
		tokens = assign_value(tokens); // Darle valores a los tokens
		// Mirar si hay errores

		
		printf("Comando ingresado: %s\n", input);

		free(input);
	}
}

int	main(void)
{
	minishell_loop();
	return (0);
}
