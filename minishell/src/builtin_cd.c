/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 20:34:45 by racasado          #+#    #+#             */
/*   Updated: 2025/04/07 20:37:05 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_env_var(char **envp, const char *var)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(var);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, len) == 0
			&& envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static void	update_env_var(t_minishell *m, char *key, char *value)
{
	char	*new_var;
	int		pos;

	new_var = ft_strjoin(key, "=");
	new_var = ft_strjoin(new_var, value);
	pos = find_env_var(m->envp, key);
	if (pos != -1)
	{
		free(m->envp[pos]);
		m->envp[pos] = new_var;
	}
}

static void	update_pwd(t_minishell *minishell)
{
	char	cwd[CWD_SIZE];
	int		pos_oldpwd;

	if (!getcwd(cwd, sizeof(cwd)))
		return (perror("Minishell: getcwd"));
	pos_oldpwd = find_env_var(minishell->envp, PWD);
	if (pos_oldpwd != -1)
		update_env_var(minishell, "OLDPWD",
			minishell->envp[pos_oldpwd] + ft_strlen(PWD) + 1);
	update_env_var(minishell, PWD, cwd);
}

static char	*build_full_path(char *path)
{
	char	cwd[CWD_SIZE];
	char	*temp;
	char	*full_path;

	if (!path)
		return (NULL);
	if (path[0] == '/' || (path[0] == '.' && (path[1] == '/'
				|| path[1] == '.')))
		return (ft_strdup(path));
	if (!getcwd(cwd, sizeof(cwd)))
		return (perror("Minishell: getcwd"), NULL);
	temp = ft_strjoin(cwd, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, path);
	free(temp);
	return (full_path);
}

int	builtin_cd(t_minishell *minishell, t_command *cmd)
{
	char	*path;
	char	*full_path;

	if (!cmd->args[1])
		return (ft_putendl_fd("Minishell: cd: usage: cd <path>", 2), 2);
	if (cmd->args[2])
		return (ft_putendl_fd("Minishell: cd: too many arguments", 2), 1);
	path = cmd->args[1];
	full_path = build_full_path(path);
	if (!full_path)
		return (1);
	if (chdir(full_path) != 0)
	{
		free(full_path);
		return (perror("Minishell: cd"), 1);
	}
	update_pwd(minishell);
	free(full_path);
	return (0);
}
