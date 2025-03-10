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

#include "minishell.h"

static void	initialize_main(t_minishell **minishell, char **envp)
{
	setup_signals();
	rl_catch_signals = 0;
	*minishell = minishell_builder(envp);
	if (!minishell)
	{
		// Throw error message
		exit(1);
	}
}

static int	check_input_exit(char *input)
{
	if (!input)
	{
		handle_eof();
		return (1);
	}
	return (0);
}

static int	check_input_empty(char *input)
{
	if (*input == '\0')
	{
		free(input);
		return (1);
	}
	return (0);
}

//print_commands(minishell->commands);
//free_tokens(minishell->tokens); !!CUIDADO!!
/* PROBLEM WHEN WE FREE TOKENS THE OUTFILE IS A POINTER
TO TOKENS SO IF WE FREE TOKENS WE REMOVE THE POINTER
FROM THE COMMAND STRUCTURE CAUSING SOME ISSUE WE SHOULD
DUPLICATE WITH STRDUP OR SOMETHING LIKE THAT */
static void	minishell_loop(char **envp)
{
	char		*input;
	t_minishell	*minishell;

	initialize_main(&minishell, envp);
	while (get_signal() && minishell)
	{
		if (isatty(STDIN_FILENO) == 0)
			dup2(open("/dev/tty", O_RDONLY), STDIN_FILENO);
		input = readline("Minishell> ");
		if (check_input_exit(input))
			break ;
		if (check_input_empty(input))
			continue ;
		add_history(input); //printf("Comando ingresado: %s\n", input);
		minishell->tokens = tokenize_input(input, minishell); // free input
		if (minishell->tokens)
		{
			minishell->commands = parse_tokens(minishell->tokens);
			if (command_executer(minishell)) // Error in command execution
				printf("ERROR IN COMMANDS\n");
		}
		// minishell_reset_loop(minishell); // Doesn't work in every case yet
		// echo hola | env | export | echo hola > outfile
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
