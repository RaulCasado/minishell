
#include "minishell.h"

void	write_strs(char **strs, char nl, char nll, char fd)
{
	while (*strs)
	{
		ft_putstr_fd(*strs, fd);
		strs++;
		if (*strs)
		{
			if (nll)
				ft_putchar_fd('\n', fd);
			else
				ft_putchar_fd(' ', fd);
		}
	}
	if (nl)
		ft_putchar_fd('\n', fd);
}

void	write_str(char *str, char fd)
{
	ft_putendl_fd(str, fd);
}
