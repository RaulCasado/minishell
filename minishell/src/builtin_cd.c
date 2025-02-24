// IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT
// PLS READ builtin_unset comments before touching this file
// IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT

// IF YOU GET ANY SOLUTIONS FOR THE PROBLEM EXPLAINED IN THE COMMENT ABOVE
// AND CHECK LINE 50 KINDA SUS AMONG US TUN TUN TUN TUN TURUN TUN TUN TUN TUN
// FIX IT AND REMOVE THE COMMENT :D
#include "minishell.h"

/*
	- Check Access() to new directory
	- Move to Directory somehow
	- We probably need to change the envp variable for PWD
*/

static int find_env_var(char **envp, const char *var)
{
    int i;
    size_t len;

    i = 0;
    len = ft_strlen(var);
    while (envp[i])
    {
        if (ft_strncmp(envp[i], var, len) == 0 && envp[i][len] == '=')
            return (i);
        i++;
    }
    return (-1);
}

static void update_env_var(t_minishell *minishell, char *key, char *value)
{
    char *new_var;
    int pos;

    new_var = ft_strjoin(key, "=");
    new_var = ft_strjoin(new_var, value);
    pos = find_env_var(minishell->envp, key);
    if (pos != -1)
    {
		//idk if we should free this
        free(minishell->envp[pos]);
        minishell->envp[pos] = new_var;
    }
}

static void update_pwd(t_minishell *minishell)
{
    char cwd[1024];
    int pos_oldpwd;

    if (!getcwd(cwd, sizeof(cwd)))
    {
        perror("Minishell: getcwd");
        return;
    }
    pos_oldpwd = find_env_var(minishell->envp, "PWD");
    if (pos_oldpwd != -1)
        update_env_var(minishell, "OLDPWD", minishell->envp[pos_oldpwd] + 4);
    update_env_var(minishell, "PWD", cwd);
}

static int is_valid_path(char *path)
{
    if (!path)
        return (0);
    if (path[0] == '/' || (path[0] == '.' && (path[1] == '/' || path[1] == '.')))
        return (1);
    return (0);
}

char builtin_cd(t_minishell *minishell, t_command *command)
{
    char *path;

    if (!command->args[1] || command->args[2])
    {
        ft_putendl_fd("Minishell: cd: usage: cd <absolute|relative path>", 2);
        return (1);
    }
    path = command->args[1];
    if (!is_valid_path(path))
    {
        ft_putendl_fd("Minishell: cd: only absolute or relative paths allowed", 2);
        return (1);
    }
    if (chdir(path) != 0)
    {
        perror("Minishell: cd");
        return (1);
    }
    update_pwd(minishell);
    return (0);
}
