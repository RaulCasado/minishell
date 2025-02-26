#include "minishell.h"

char command_executer(t_minishell *minishell)
{
    t_command   *cmd;
    size_t      cmd_len;
    int         saved_stdin;
    int         saved_stdout;

    cmd = minishell->commands;
    while (cmd)
    {
        // Save original stdin/stdout
        saved_stdin = dup(STDIN_FILENO);
        saved_stdout = dup(STDOUT_FILENO);
        
        // Set up redirections
        handle_redirections(cmd);
        
        cmd_len = ft_strlen(cmd->args[0]);
        
        // Execute the command based on its name
        if (cmd_len == 4 && !ft_strncmp(cmd->args[0], "echo", 4))
            builtin_echo(minishell, cmd);
        else if (cmd_len == 2 && !ft_strncmp(cmd->args[0], "cd", 2))
            builtin_cd(minishell, cmd);
        else if (cmd_len == 3 && !ft_strncmp(cmd->args[0], "pwd", 3))
            builtin_pwd(minishell, cmd);
        else if (cmd_len == 6 && !ft_strncmp(cmd->args[0], "export", 6))
            builtin_export(minishell, cmd);
        else if (cmd_len == 5 && !ft_strncmp(cmd->args[0], "unset", 5))
            builtin_unset(minishell, cmd);
        else if (cmd_len == 3 && !ft_strncmp(cmd->args[0], "env", 3))
            builtin_env(minishell, cmd);
        else if (cmd_len == 4 && !ft_strncmp(cmd->args[0], "exit", 4))
            builtin_exit(minishell, cmd);
        else
            command_process(minishell, cmd);
        
        // Restore original stdin/stdout
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);

        cmd = cmd->next;
    }
    return (0);
}