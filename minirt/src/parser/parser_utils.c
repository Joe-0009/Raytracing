#include "../includes/minirt_app.h"

/**
 * Free an array of strings
 *
 * @param tokens Array of strings to free
 */
void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
