/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: droura-s <droura-s@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 17:30:04 by racasado          #+#    #+#             */
/*   Updated: 2025/03/05 11:39:31 by droura-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void	free_envp(char **envp)
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

void	free_minishell(t_minishell *minishell)
{
	rl_clear_history();
	free_commands(minishell->commands);
	free_tokens(minishell->tokens);
	free(minishell->tokens);
	free_envp(minishell->envp);
	free(minishell);
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
