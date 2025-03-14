/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_decider.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 18:52:54 by racasado          #+#    #+#             */
/*   Updated: 2025/03/14 18:53:02 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_command(t_minishell *minishell, t_command *cmd)
{
	if (ft_strcmp(cmd->args[0], ECHO_LOWER) == 0)
		builtin_echo(minishell, cmd);
	else if (ft_strcmp(cmd->args[0], CD_LOWER) == 0)
		builtin_cd(minishell, cmd);
	else if (ft_strcmp(cmd->args[0], PWD_LOWER) == 0)
		builtin_pwd(minishell, cmd);
	else if (ft_strcmp(cmd->args[0], EXPORT_LOWER) == 0)
		builtin_export(minishell, cmd);
	else if (ft_strcmp(cmd->args[0], UNSET_LOWER) == 0)
		builtin_unset(minishell, cmd);
	else if (ft_strcmp(cmd->args[0], ENV_LOWER) == 0)
		builtin_env(minishell, cmd);
	else if (ft_strcmp(cmd->args[0], EXIT_LOWER) == 0)
		builtin_exit(minishell, cmd);
	else
		command_process(minishell, cmd);
}