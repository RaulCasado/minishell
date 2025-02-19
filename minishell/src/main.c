/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: droura-s <droura-s@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:20:26 by racasado          #+#    #+#             */
/*   Updated: 2025/02/19 18:21:41 by droura-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// main principal del minichell

#include "minishell.h"

static void minishell_loop(char **envp)
{
	char	*input;
	t_token	*tokens;
	t_minishell	*minishell;
	t_command	*commands;

	setup_signals();
	rl_catch_signals = 0;
	// minishell = new minishell;

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
		// minishell.tokens = tokens;

		add_history(input);
		// If the command is exi it also count :c
		if (ft_strlen(input) == 4 && ft_strncmp(input, "exit", 5) == 0)
		{
			free(input);
			printf("Saliendo de Minishell...\n");
			set_signal(0);
			break;
		}

		printf("Comando ingresado: %s\n", input);
		tokens = tokenize_input(input);
		if (tokens)
		{
			// minishell.commands = build_commands(tokens)
			(void) 0; // Execute command
			commands = parse_tokens(tokens);
			print_commands(commands);
			printf("FIN COMANDOS INGRESADOS\n\n");
			free_tokens(tokens);
			tokens = NULL;
			if (command_executer(commands, envp)) // Error in some command execution
				printf("ERROR IN COMMANDS\n");
		}
		else
		{
			// Free something
		}

		free(input);
	}
}


int	main(int argc, char **argv, char **envp)
{
	(void)argv; // i dont know if we will need this in the future
	if (argc > 1)
	{
		ft_putendl_fd("Error: Minishell no acepta argumentos", 2);
		return (1);
	}
	minishell_loop(envp);
	return (0);
}