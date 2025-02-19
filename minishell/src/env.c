
#include "minishell.h"

char	*get_env(char *path)
{
	char	*str;

	str = getenv(path);
	if (!str)
		return (NULL);
	return (str);
}
