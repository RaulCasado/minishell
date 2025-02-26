#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

static int open_output_file(char *file, int append)
{
    int fd;
    
    // Print current working directory and file path for debugging
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("DEBUG: Current directory: %s\n", cwd);
    }
    printf("DEBUG: Attempting to open output file: %s (append mode: %d)\n", file, append);
    
    if (append == 2) // >> (append)
        fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (append == 1) // > (overwrite)
        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else
        return (-1);
    
    if (fd == -1) {
        printf("DEBUG: Error opening file: %s (errno: %d)\n", strerror(errno), errno);
        perror("Minishell: open");
    } else {
        printf("DEBUG: Successfully opened file with fd: %d\n", fd);
    }
    
    return (fd);
}

void handle_redirections(t_command *cmd)
{
    int fd;
    
    printf("DEBUG: Handling redirections for command\n");
    
    // Safety check to avoid using corrupted strings
    if (cmd->outfile && (cmd->outfile[0] == '\0' || 
        (unsigned char)cmd->outfile[0] > 127)) {
        printf("ERROR: Corrupted outfile pointer detected\n");
        return;
    }
    
    printf("DEBUG: outfile: %s, append: %d\n", 
           cmd->outfile ? cmd->outfile : "(null)", cmd->append);
    
    // Handle output redirection
    if (cmd->outfile)
    {
        // Additional safety check
        if (access(cmd->outfile, F_OK) == -1 && errno != ENOENT) {
            printf("DEBUG: Invalid path: %s\n", strerror(errno));
            return;
        }
        
        fd = open_output_file(cmd->outfile, cmd->append);
        if (fd == -1)
            return;
        
        printf("DEBUG: Redirecting STDOUT to fd %d\n", fd);
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            perror("Minishell: dup2");
            close(fd);
            return;
        }
        close(fd);
    }
    // Handle input redirection (keep this after the debug output above)
    if (cmd->infile)
    {
        fd = open(cmd->infile, O_RDONLY);
        if (fd == -1)
        {
            perror("Minishell: open");
            return;
        }
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            perror("Minishell: dup2");
            close(fd);
            return;
        }
        close(fd);
    }
}
