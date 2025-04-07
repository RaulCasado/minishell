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

static int	minishell_loop(char **envp)
{
	char		*input;
	t_minishell	*minishell;
	int		cmd_result;
	int		exit_status;

	initialize_main(&minishell, envp);
	while (get_signal() && minishell)
	{
/* 		if (isatty(STDIN_FILENO) == 0)
			dup2(open("/dev/tty", O_RDONLY), STDIN_FILENO); */
		input = readline("Minishell> ");
		if (check_input_exit(input))
			break ;
		if (check_input_empty(input))
			continue ;
		add_history(input);
		minishell->tokens = tokenize_input(input, minishell);
		//print_tokens(minishell->tokens);
		if (minishell->tokens)
		{
			minishell->commands = parse_tokens(minishell->tokens);
			if (minishell->commands)
			{
				cmd_result = command_executer(minishell);
				minishell->exit_code = cmd_result;
			}
		}
		minishell_reset_loop(input, minishell);
	}
	exit_status = minishell->exit_code;
	free_minishell(minishell);
	return (exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	int	exit_status;

	(void)argv;
	if (argc > 1)
	{
		ft_putendl_fd("Error: Minishell no acepta argumentos", 2);
		return (1);
	}
	exit_status = minishell_loop(envp);
	return (exit_status);
}
