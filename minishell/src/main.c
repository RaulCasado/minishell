/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:20:26 by racasado          #+#    #+#             */
/*   Updated: 2025/02/10 17:45:23 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// main principal del minichell

#include "minishell.h"

void minishell_loop() {
    char *input;

    while (1) {
        input = readline("Minishell> ");
        if (!input)
        {
            printf("\nSaliendo de Minishell...\n");
            break;
        }
        if (*input)
            add_history(input);
        if (strcmp(input, "exit") == 0)
        {
            free(input);
            printf("Saliendo de Minishell...\n");
            exit(0);
        }
        printf("Comando ingresado: %s\n", input);

        free(input);
    }
}

int main() {
    minishell_loop();
	ft_putstr_fd("Hello, World!\n", 1);
    return 0;
}
