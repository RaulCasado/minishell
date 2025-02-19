
#include "minishell.h"

void	write_strs(char **strs, char newline, char fd)
{
	while (*strs)
	{
		ft_putstr_fd(*strs, fd);
		strs++;
		if (*strs)
			ft_putchar_fd(' ', fd);
	}
	if (newline)
		ft_putchar_fd('\n', fd);
}
