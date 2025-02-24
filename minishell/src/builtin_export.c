// this function is used to export environment variables
// it can also be used to print the environment variables
// if no arguments are passed
// it will print the environment variables in sorted order
// if arguments are passed, it will add or replace the environment variables


// READ UNSET BUILTIN COMMENTS FIRST PLS

#include "minishell.h"

static char **add_env_var(t_minishell *minishell, char *new_var)
{
    int i;
    char **new_envp;

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
static int find_env_var(char **envp, const char *var)
{
    int i;
    size_t len;
    char *equal_sign;

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

static int is_valid_identifier(const char *var)
{
    int i;

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

static void print_sorted_env(char **envp)
{
    int i;
    int j;
    char *tmp;

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

static void add_or_replace_env_var(t_minishell *minishell, char *var)
{
    int pos;
    char *equal_sign;

    equal_sign = ft_strchr(var, '=');
    if (!equal_sign)
        return;  // Solo export VAR (sin `=`), no hace nada

    pos = find_env_var(minishell->envp, var);
    if (pos != -1)
    {
		// we found it so we need to replace it
        free(minishell->envp[pos]);
        minishell->envp[pos] = var;
    }
    else
        minishell->envp = add_env_var(minishell, var);
}

char builtin_export(t_minishell *minishell, t_command *command)
{
    int i;

    if (!command->args[1])
    {
        print_sorted_env(minishell->envp);
        return (0);
    }
    i = 1;
    while (command->args[i])
    {
        if (!is_valid_identifier(command->args[i]))
            printf("Minishell: export: `%s': not a valid identifier\n", command->args[i]);
        else
            add_or_replace_env_var(minishell, command->args[i]);
        i++;
    }
    return (0);
}
