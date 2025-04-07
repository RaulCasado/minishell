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

static int	process_iteration(t_minishell *minishell)
{
	char	*input;

	input = readline("Minishell> ");
	if (check_input_exit(input))
		return (1);
	if (check_input_empty(input))
		return (0);
	add_history(input);
	minishell->tokens = tokenize_input(input, minishell);
	if (minishell->tokens)
	{
		minishell->commands = parse_tokens(minishell->tokens);
		if (minishell->commands)
			minishell->exit_code = command_executer(minishell);
	}
	minishell_reset_loop(input, minishell);
	return (0);
}

static int	minishell_loop(char **envp)
{
	t_minishell	*minishell;
	int			exit_status;

	initialize_main(&minishell, envp);
	while (get_signal() && minishell)
	{
		if (process_iteration(minishell) == 1)
			break ;
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
