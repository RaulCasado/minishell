/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builder.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:32:20 by racasado          #+#    #+#             */
/*   Updated: 2025/03/14 17:32:21 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_minishell	*minishell_builder(char **envp)
{
	t_minishell	*minishell;
	char		**new_envp;

	minishell = malloc(sizeof(struct s_minishell));
	if (!minishell)
		return (NULL);
	minishell->commands = NULL;
	minishell->tokens = NULL;
	minishell->exit_code = 0;
	new_envp = copy_envp(envp);
	if (!new_envp)
	{
		free(minishell);
		return (NULL);
	}
	minishell->envp = new_envp;
	return (minishell);
}

void	minishell_reset_loop(char *input, t_minishell *minishell)
{
	free(input);
	free_commands(minishell->commands);
	minishell->commands = NULL;
	free_tokens(minishell->tokens);
	minishell->tokens = NULL;
}
