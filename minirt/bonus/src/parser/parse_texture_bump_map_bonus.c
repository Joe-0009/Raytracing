#include "../includes/minirt_app_bonus.h"
#include "../includes/parser_bonus.h"

void	init_texture_bump_struct(t_surface_map *texture, t_surface_map *bump)
{
	texture->is_active = 0;
	texture->data = NULL;
	texture->mlx_img = NULL;
	texture->width = 0;
	texture->height = 0;
	texture->path = NULL;
	texture->map_type = MAP_TYPE_TEXTURE;
	bump->is_active = 0;
	bump->data = NULL;
	bump->mlx_img = NULL;
	bump->width = 0;
	bump->height = 0;
	bump->path = NULL;
	bump->map_type = MAP_TYPE_BUMP;
}

/*
** Parse texture string that can contain texture maps
** Syntax: "txm:earthmap1k.xpm"
*/
int	parse_texture(char *str, t_surface_map *texture)
{
	char	*texture_file;

	texture_file = NULL;
	if (ft_strncmp(str, "txm:", 4) == 0)
		texture_file = str + 4;
	else
		return (printf("Not the right format for sphere object !\n"), FALSE);
	if (texture_file)
	{
		texture->path = ft_strdup(texture_file);
		texture->is_active = 1;
	}
	else
		return (printf("Not the right format for texture files !\n"), FALSE);
	return (TRUE);
}

/*
** Parse texture string that can contain bump
** Syntax: "bum:earthbump1k.xpm
*/

int	parse_bump(char *str, t_surface_map *bump)
{
	char	*bump_file;

	bump_file = NULL;
	if (ft_strncmp(str, "bum:", 4) == 0)
		bump_file = str + 4;
	else
		return (printf("Not the right format for bump files !\n"), FALSE);
	if (bump_file)
	{
		bump->path = ft_strdup(bump_file);
		if (!bump->path)
			return (FALSE);
		bump->is_active = 1;
	}
	else
		return (printf("Not the right format for bump files !\n"), FALSE);
	return (TRUE);
}
