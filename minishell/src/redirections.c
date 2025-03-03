#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

static int open_output_file(char *file, int append)
{
    int fd;
    
    if (append == 2) // >> (append)
        fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (append == 1) // > (overwrite)
        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else
        return (-1);
    
    if (fd == -1) {
        perror("Minishell: open");
    }
    
    return (fd);
}

void handle_redirections(t_command *cmd)
{
    int fd;

    // Handle output redirection
    if (cmd->outfile)
    {
        if (cmd->outfile[0] == '\0' || (unsigned char)cmd->outfile[0] > 127) {
            return;
        }
        
        fd = open_output_file(cmd->outfile, cmd->append);
        if (fd == -1)
            return;
        
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            perror("Minishell: dup2");
            close(fd);
            return;
        }
        close(fd);
    }
    
    // Handle input redirection
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
