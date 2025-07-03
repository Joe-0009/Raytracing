#include "../includes/minirt_app_bonus.h"

void	ft_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

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

void	free_tokens(char **tokens)
{
	ft_free_array(&tokens);
}

void	ft_free_scene(t_scene **scene)
{
	if (!scene || !*scene)
		return ;
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
