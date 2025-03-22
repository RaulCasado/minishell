/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: droura-s <droura-s@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:23:31 by racasado          #+#    #+#             */
/*   Updated: 2025/03/19 12:47:42 by droura-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(const char *var)
{
	int	i;

	if (!var || (!ft_isalpha(var[0]) && var[0] != '_'))
		return (0);
	i = 1;
	while (var[i] && var[i] != '=')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_sorted_env(char **envp)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (envp[i])
	{
		j = i + 1;
		while (envp[j])
		{
			if (ft_strncmp(envp[i], envp[j], ft_strlen(envp[i])) > 0)
			{
				tmp = envp[i];
				envp[i] = envp[j];
				envp[j] = tmp;
			}
			j++;
		}
		printf("declare -x %s\n", envp[i]);
		i++;
	}
}

static char *remove_quotes(char *str)
{
    size_t  len;
    char    *new_arg;
    char    *equal_sign;
    char    *value;

    if (!str)
        return (NULL);
    equal_sign = ft_strchr(str, '=');
    if (!equal_sign || !(equal_sign + 1))
        return (str);
    value = equal_sign + 1;
    if (value[0] == DOUBLE_MARK && value[ft_strlen(value) - 1] == DOUBLE_MARK)
    {
        len = ft_strlen(value) - 2;
        new_arg = malloc(sizeof(char) * (equal_sign - str + 1 + len + 1));

        if (!new_arg)
            return (str);
        ft_memcpy(new_arg, str, equal_sign - str + 1);
        ft_memcpy(new_arg + (equal_sign - str + 1), value + 1, len);
        new_arg[equal_sign - str + 1 + len] = '\0';
        return (new_arg);
    }
    return (str);
}
/*
Remove quotes needs more work because it's leaking


==90321== 10 bytes in 1 blocks are definitely lost in loss record 3 of 64
==90321==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==90321==    by 0x10CE2B: remove_quotes (in /mnt/c/Users/danie/minichell/minishell/minishell)
==90321==    by 0x10CED9: process_export_arg (in /mnt/c/Users/danie/minichell/minishell/minishell)
==90321==    by 0x10CFD2: builtin_export (in /mnt/c/Users/danie/minichell/minishell/minishell)
==90321==    by 0x10DAE6: execute_command (in /mnt/c/Users/danie/minichell/minishell/minishell)
==90321==    by 0x10C1F7: command_executer (in /mnt/c/Users/danie/minichell/minishell/minishell)
==90321==    by 0x10966A: minishell_loop (in /mnt/c/Users/danie/minichell/minishell/minishell)
==90321==    by 0x109714: main (in /mnt/c/Users/danie/minichell/minishell/minishell)
==90321== 
==90321== LEAK SUMMARY:
==90321==    definitely lost: 10 bytes in 1 blocks
==90321==    indirectly lost: 0 bytes in 0 blocks
==90321==      possibly lost: 0 bytes in 0 blocks
==90321==    still reachable: 208,178 bytes in 222 blocks
==90321==         suppressed: 0 bytes in 0 blocks
==90321== Reachable blocks (those to which a pointer was found) are not shown.
==90321== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==90321== 
==90321== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
*/

static void	process_export_arg(t_minishell *minishell, char *arg,
	int *exit_code)
{
	char	*new_arg;
	char	*tmp;

	new_arg = remove_quotes(arg);
	arg = new_arg;/**/
	printf("\t\t%s %s\n", arg, new_arg);
	if (arg != new_arg) // ?
	{
		tmp = arg;
		arg = new_arg;
		free(tmp);
	}
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("Minishell: export: `", STDERR_FILENO);
		*exit_code = 1;
		return ;
	}
	if (add_or_replace_env_var(minishell, arg) != 0)
	{
		ft_putendl_fd("Minishell: export: memory allocation failed",
			STDERR_FILENO);
		*exit_code = 1;
	}
}

int	builtin_export(t_minishell *minishell, t_command *command)
{
	int	exit_code;
	int	i;

	exit_code = 0;
	if (!command->args[1])
	{
		print_sorted_env(minishell->envp);
		return (0);
	}
	i = 1;
	while (command->args[i])
	{
		process_export_arg(minishell, command->args[i], &exit_code);
		i++;
	}
	return (exit_code);
}
