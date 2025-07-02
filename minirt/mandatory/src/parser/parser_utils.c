#include "../includes/minirt_app.h"

/**
 * Safe free function that sets pointer to NULL after freeing
 * Prevents double-free errors and dangling pointers
 *
 * @param ptr Pointer to the memory to free
 */
void	ft_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

/**
 * Free an array of strings and set the array pointer to NULL
 *
 * @param tokens Pointer to array of strings to free
 */
void	ft_free_array(char ***tokens)
{
	int	i;

	if (!tokens || !*tokens)
		return ;
	i = 0;
	while ((*tokens)[i])
	{
		ft_free((void **)&(*tokens)[i]);
		i++;
	}
	ft_free((void **)tokens);
}

/**
 * Free an array of strings (legacy wrapper for ft_free_array)
 *
 * @param tokens Array of strings to free
 */
void	free_tokens(char **tokens)
{
	ft_free_array(&tokens);
}

/**
 * Clean up scene resources and set pointer to NULL
 *
 * @param scene Pointer to scene to free
 */
void	ft_free_scene(t_scene **scene)
{
	if (!scene || !*scene)
		return ;
	/* In this implementation, scene contains only static arrays and values,
		* so we only need to free the scene structure itself */
	ft_free((void **)scene);
}

int	is_empty_line(const char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n'
			&& line[i] != '\r')
			return (FALSE);
		i++;
	}
	return (TRUE);
}
