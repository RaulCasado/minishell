/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: droura-s <droura-s@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:20:26 by racasado          #+#    #+#             */
/*   Updated: 2025/02/11 19:05:51 by droura-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// main principal del minichell

#include "minishell.h"

void minishell_loop() {
    char	*input;
    t_token	*tokens;

    setup_signals();

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

        if (ft_strncmp(input, "exit", ft_strlen(input)) == 0)
        {
            free(input);
            printf("Saliendo de Minishell...\n");
            set_signal(0);
            break;
        }

        tokens = tokenize_input(input);
		if (tokens)
		{
			(void) 0; // Execute command
			free_tokens(tokens);
			tokens = NULL;
		}

        printf("Comando ingresado: %s\n", input);

        free(input);
    }
}


int	main(void)
{
	minishell_loop();
	return (0);
}