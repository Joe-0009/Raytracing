
#include "libft.h"

void	ft_free(char **buffer)
{
	if (*buffer)
	{
		free(*buffer);
		*buffer = NULL;
	}
} 
