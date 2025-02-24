/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: droura-s <droura-s@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 17:30:04 by racasado          #+#    #+#             */
/*   Updated: 2025/02/24 14:09:18 by droura-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static void	free_envp(char **envp)
{
	size_t	i;

	i = 0;
	while(envp[i])
		free(envp[i++]);
	free(envp);
}

static void	free_command(t_command *command)
{
	size_t	i;

	i = 0;
	while (command->args[i])
		free(command->args[i++]);
	free(command->args);
	if (command->infile)
		free(command->infile);
	if (command->outfile)
		free(command->outfile);
	free(command);
}

void	free_commands(t_command *commands)
{
	t_command	*tmp_cmd;
	size_t		i;

	while (commands)
	{
		tmp_cmd = commands;
		commands = commands->next;
		free_command(tmp_cmd);
	}
}

/*
==820436== Invalid free() / delete / delete[] / realloc()
==820436==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==820436==    by 0x40162A: free_minishell (in /home/droura-s/Minichell/minishell/minishell)
==820436==    by 0x403668: builtin_exit (in /home/droura-s/Minichell/minishell/minishell)
==820436==    by 0x40338C: command_executer (in /home/droura-s/Minichell/minishell/minishell)
==820436==    by 0x40139A: minishell_loop (in /home/droura-s/Minichell/minishell/minishell)
==820436==    by 0x40125C: main (in /home/droura-s/Minichell/minishell/minishell)
==820436==  Address 0x4b92e60 is 0 bytes inside a block of size 48 free'd
==820436==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==820436==    by 0x4015EA: free_command (in /home/droura-s/Minichell/minishell/minishell)
==820436==    by 0x401543: free_commands (in /home/droura-s/Minichell/minishell/minishell)
==820436==    by 0x40161D: free_minishell (in /home/droura-s/Minichell/minishell/minishell)
==820436==    by 0x403668: builtin_exit (in /home/droura-s/Minichell/minishell/minishell)
==820436==    by 0x40338C: command_executer (in /home/droura-s/Minichell/minishell/minishell)
==820436==    by 0x40139A: minishell_loop (in /home/droura-s/Minichell/minishell/minishell)
==820436==    by 0x40125C: main (in /home/droura-s/Minichell/minishell/minishell)
==820436==  Block was alloc'd at
==820436==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==820436==    by 0x402FCC: create_command (in /home/droura-s/Minichell/minishell/minishell)
==820436==    by 0x402E51: parse_tokens (in /home/droura-s/Minichell/minishell/minishell)
==820436==    by 0x40134B: minishell_loop (in /home/droura-s/Minichell/minishell/minishell)
==820436==    by 0x40125C: main (in /home/droura-s/Minichell/minishell/minishell)
==820436== 
==820436== 
==820436== HEAP SUMMARY:
==820436==     in use at exit: 208,205 bytes in 223 blocks
==820436==   total heap usage: 507 allocs, 285 frees, 239,581 bytes allocated
*/

void	free_minishell(t_minishell *minishell)
{
	rl_clear_history();
	free_commands(minishell->commands);
	free(minishell->commands);
	free_tokens(minishell->tokens);
	free(minishell->tokens);
	// free_envp(minishell->envp);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}
