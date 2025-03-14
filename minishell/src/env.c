/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:31:23 by racasado          #+#    #+#             */
/*   Updated: 2025/03/14 17:31:41 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env(char *name, char **envp)
{
	int		i;
	size_t	len;

	if (name == NULL || envp == NULL)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (&envp[i][len + 1]);
		i++;
	}
	return (NULL);
}

char	**copy_envp(char **envp)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			free_ptr_ptr(new_envp, i);
			return (NULL);
		}
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

static char	**add_env_var(t_minishell *minishell, char *new_var)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (minishell->envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 2));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (minishell->envp[i])
	{
		new_envp[i] = minishell->envp[i];
		i++;
	}
	new_envp[i] = ft_strdup(new_var);
	new_envp[i + 1] = NULL;
	free(minishell->envp);
	return (new_envp);
}

static int	find_env_var(char **envp, const char *var)
{
	int		i;
	size_t	len;
	char	*equal_sign;

	equal_sign = ft_strchr(var, '=');
	if (equal_sign)
		len = (size_t)(equal_sign - var);
	else
		len = ft_strlen(var);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, len) == 0 && envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	add_or_replace_env_var(t_minishell *minishell, char *var)
{
	int		pos;
	char	*equal_sign;
	char	**new_envp;
	char	*new_value;

	equal_sign = ft_strchr(var, '=');
	if (!equal_sign)
		return (0);
	pos = find_env_var(minishell->envp, var);
	if (pos != -1)
	{
		new_value = ft_strdup(var);
		if (!new_value)
			return (1);
		free(minishell->envp[pos]);
		minishell->envp[pos] = new_value;
	}
	else
	{
		new_envp = add_env_var(minishell, var);
		if (!new_envp)
			return (1);
		minishell->envp = new_envp;
	}
	return (0);
}
