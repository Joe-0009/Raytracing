/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 22:13:23 by yrachidi          #+#    #+#             */
/*   Updated: 2025/08/21 22:13:24 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt_app.h"

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
