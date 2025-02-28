/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:20:26 by racasado          #+#    #+#             */
/*   Updated: 2025/02/28 17:24:23 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// main principal del minichell

#include "minishell.h"

static void minishell_loop(char **envp)
{
	char	*input;
	t_minishell	*minishell;

	setup_signals();
	rl_catch_signals = 0;
	minishell = minishell_builder(envp);
	if (!minishell)
		return ; // Throw error message
	while (get_signal() && minishell)
	{
		minishell->tokens = NULL;
		minishell->commands = NULL;
		input = readline("Minishell> ");

		if (!input) {
			handle_eof();
			break;
		}
		if (*input == '\0') {
			free(input);
			continue;
		}

		add_history(input);

		//printf("Comando ingresado: %s\n", input);
		minishell->tokens = tokenize_input(input, minishell);
		free(input);

		if (minishell->tokens)
		{
			minishell->commands = parse_tokens(minishell->tokens);
			//print_commands(minishell->commands);
			//printf("FIN COMANDOS INGRESADOS\n\n");
			//free_tokens(minishell->tokens); !!CUIDADO!!
			/*PROBLEM WHEN WE FREE TOKENS THE OUTFILE IS A POINTER TO TOKENS SO IF WE FREE TOKENS WE REMOVE THE POINTER FROM THE COMMAND
			STRUCTURE CAUSING SOME ISSUE WE SHOULD DUPLICATE WITH STRDUP OR SOMETHING LIKE THAT*/
			minishell->tokens = NULL;
			if (command_executer(minishell)) // Error in some command execution
				printf("ERROR IN COMMANDS\n");
		}
	}
	free_minishell(minishell);
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