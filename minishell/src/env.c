
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
