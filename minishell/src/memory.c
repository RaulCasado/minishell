/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 17:30:04 by racasado          #+#    #+#             */
/*   Updated: 2025/03/26 12:09:17 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	// Free extra_outfiles if they exist.
	if (command->extra_outfiles)
		ft_free_split(command->extra_outfiles);
	free(command);
}

void	free_commands(t_command *commands)
{
	t_command	*tmp_cmd;

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
	//free(minishell->tokens);
	free_envp(minishell->envp);
	free(minishell);
}

void	free_token(t_token *token)
{
	free(token->value);
	free(token);
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
