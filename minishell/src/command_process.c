
#include "minishell.h"
#include <sys/wait.h>

#define MSGSIZE 512 


/*	This file is a prototype to build the Pipex	*/

void ft_free_split(char **split)
{
	int i;
	
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}
char *find_command_path(char *cmd, char **envp)
{
    char *path_env, *path, *full_path;
    char **paths;
    int i;
    
    // Handle builtin commands or absolute/relative paths
    if (cmd[0] == '/' || cmd[0] == '.')
        return (ft_strdup(cmd));
    
    // Get PATH environment variable
    path_env = get_env("PATH", envp);
    if (!path_env)
        return (NULL);
    
    // Split PATH into individual directories
    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);
    
    // Try each directory
    i = 0;
    while (paths[i])
    {
        path = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(path, cmd);
        free(path);
        
        if (access(full_path, X_OK) == 0)
        {
            // Clean up and return found path
            ft_free_split(paths);
            return (full_path);
        }
        
        free(full_path);
        i++;
    }
    
    ft_free_split(paths);
    return (NULL);
}


char command_process(t_minishell *minishell, t_command *command)
{
    char *path;
    
    // Handle command not found
    if (!command->args[0])
        return (1);
    
    // Get command path
    path = find_command_path(command->args[0], minishell->envp);
    if (!path)
    {
        ft_putstr_fd(command->args[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        exit(127);
    }
    
    // Execute command with all arguments and environment
    execve(path, command->args, minishell->envp);
    
    // If execve fails
    perror("minishell");
    free(path);
    exit(1);
    
    return (0); // This line is never reached
}