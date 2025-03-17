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
		add_history(input);
		minishell->tokens = tokenize_input(input, minishell);
		if (minishell->tokens)
		{
			minishell->commands = parse_tokens(minishell->tokens);
			if (command_executer(minishell))
				printf("ERROR IN COMMANDS\n");
		}
		minishell_reset_loop(minishell); // Doesn't work in every case yet
		// echo hola | env | export | echo hola > outfile
		// echo hola | env | export
	}
	free_minishell(minishell);
}

/*
Minishell> echo hola | echo hola | echo hola


Minishell> echo hola | env | export

^D

==8213== HEAP SUMMARY:
==8213==     in use at exit: 208,317 bytes in 228 blocks
==8213==   total heap usage: 507 allocs, 279 frees, 241,193 bytes allocated
==8213== 
==8213== 139 (48 direct, 91 indirect) bytes in 1 blocks are definitely lost in loss record 23 of 67
==8213==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==8213==    by 0x10D28A: create_command (in /mnt/c/Users/danie/minichell/minishell/minishell)
==8213==    by 0x10B99A: handle_pipe (in /mnt/c/Users/danie/minichell/minishell/minishell)
==8213==    by 0x10BAD8: process_token (in /mnt/c/Users/danie/minichell/minishell/minishell)
==8213==    by 0x10BB59: parse_tokens (in /mnt/c/Users/danie/minichell/minishell/minishell)
==8213==    by 0x10965A: minishell_loop (in /mnt/c/Users/danie/minichell/minishell/minishell)
==8213==    by 0x10970D: main (in /mnt/c/Users/danie/minichell/minishell/minishell)
==8213== 
==8213== LEAK SUMMARY:
==8213==    definitely lost: 48 bytes in 1 blocks
==8213==    indirectly lost: 91 bytes in 5 blocks
==8213==      possibly lost: 0 bytes in 0 blocks
==8213==    still reachable: 208,178 bytes in 222 blocks
==8213==         suppressed: 0 bytes in 0 blocks
==8213== Reachable blocks (those to which a pointer was found) are not shown.
==8213== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==8213== 
==8213== For lists of detected and suppressed errors, rerun with: -s
==8213== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
*/

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	if (argc > 1)
	{
		ft_putendl_fd("Error: Minishell no acepta argumentos", 2);
		return (1);
	}
	minishell_loop(envp);
	return (0);
}
