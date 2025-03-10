#include "minishell.h"

int	write_strs(char **strs, char nl, char nll, int fd)
{
	char	*sep;

	sep = " ";
	if (nll)
		sep = "\n";
	while (*strs)
	{
		if (write(fd, *strs, ft_strlen(*strs)) == -1)
			return (-1);
		strs++;
		if (*strs && write(fd, sep, 1) == -1)
			return (-1);
	}
	if (nl && write(fd, "\n", 1) == -1)
		return (-1);
	return (0);
}

int	write_str(char *str, int fd)
{
	if (write(fd, str, ft_strlen(str)) == -1)
		return (-1);
	if (write(fd, "\n", 1) == -1)
		return (-1);
	return (0);
}
