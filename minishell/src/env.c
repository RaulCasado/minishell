#include "minishell.h"

char *get_env(char *name, char **envp)
{
	int i;
	size_t len;

	if (name == NULL || envp == NULL)
		return NULL;

	len = ft_strlen(name);
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
		{
			// Return the value after the '=' character
			return (&envp[i][len + 1]);
		}
		i++;
	}
	return NULL;
}

char **copy_envp(char **envp)
{
	int i = 0;
	char **new_envp;

	while (envp[i])
		i++;

	new_envp = malloc(sizeof(char *) * (i + 1));
	if (!new_envp)
		return (NULL);

	i = 0;
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		// is !new_envp => free (0..i-1)
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}
