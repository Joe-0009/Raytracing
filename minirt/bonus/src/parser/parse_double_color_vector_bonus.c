#include "../includes/minirt_app_bonus.h"
#include "../includes/parser_bonus.h"

int	parse_double(char *str, double *value)
{
	char	*dot;
	double	result;
	char	*frac_str;
	int		divisor;
	int		j;

	if (!str || !value)
		return (FALSE);
	dot = ft_strchr(str, '.');
	result = 0.0;
	if (dot)
	{
		*dot = '\0';
		result = (double)ft_atoi(str);
		frac_str = dot + 1;
		divisor = 1;
		j = -1;
		while (frac_str[++j])
			divisor *= 10;
		result += (double)ft_atoi(frac_str) / divisor;
	}
	else
		result = (double)ft_atoi(str);
	*value = result;
	return (TRUE);
}

int	parse_color(char *str, t_color3 *color)
{
	char	**tokens;
	int		success;
	int		r;
	int		g;
	int		b;

	success = TRUE;
	tokens = ft_split(str, ",");
	if (!tokens || !tokens[0] || !tokens[1] || !tokens[2])
		return (printf(ERR_COLOR_FORMAT), free_tokens(tokens), FALSE);
	r = ft_atoi(tokens[0]);
	g = ft_atoi(tokens[1]);
	b = ft_atoi(tokens[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		success = FALSE;
	if (success)
	{
		color->x = r / 255.0;
		color->y = g / 255.0;
		color->z = b / 255.0;
	}
	else
		printf(ERR_COLOR_FORMAT);
	free_tokens(tokens);
	return (success);
}

int	parse_vector(char *str, t_vec3 *vec)
{
	char	**tokens;
	double	x;
	double	y;
	double	z;

	tokens = ft_split(str, ",");
	if (!tokens || !tokens[0] || !tokens[1] || !tokens[2] || tokens[3])
	{
		printf(ERR_VECTOR_FORMAT);
		free_tokens(tokens);
		return (FALSE);
	}
	if (!parse_double(tokens[0], &x) || !parse_double(tokens[1], &y)
		|| !parse_double(tokens[2], &z))
	{
		printf(ERR_VECTOR_FORMAT);
		free_tokens(tokens);
		return (FALSE);
	}
	vec->x = x;
	vec->y = y;
	vec->z = z;
	free_tokens(tokens);
	return (TRUE);
}

int	validate_non_zero_vector(t_vec3 vec)
{
	if (vec.x == 0.0 && vec.y == 0.0 && vec.z == 0.0)
	{
		printf(ERR_VECTOR_FORMAT);
		return (FALSE);
	}
	return (TRUE);
}

int	validate_normalized_vector(t_vec3 vec)
{
	double	length;

	length = vec3_length(vec);
	if (fabs(length - 1.0) > 0.0001)
	{
		printf(ERR_VECTOR_FORMAT);
		return (FALSE);
	}
	return (TRUE);
}

void	init_texture_bump_struct(t_texture *texture, t_bump *bump)
{
	texture->has_texture = 0;
	texture->texture_data = NULL;
	texture->texture_mlx_img = NULL;
	texture->texture_width = 0;
	texture->texture_height = 0;
	texture->texture_path = NULL;
	bump->has_bump_map = 0;
	bump->bump_data = NULL;
	bump->bump_mlx_img = NULL;
	bump->bump_width = 0;
	bump->bump_height = 0;
	bump->bump_path = NULL;
}

/*
** Parse texture string that can contain texture maps
** Syntax: "txm:earthmap1k.xpm"
*/
int	parse_texture(char *str, t_texture *texture)
{
	char	*texture_file;

	texture_file = NULL;
	if (ft_strncmp(str, "txm:", 4) == 0)
		texture_file = str + 4;
	else
		return (printf("Not the right format for sphere object !\n"), FALSE);
	if (texture_file)
	{
		texture->texture_path = ft_strdup(texture_file);
		texture->has_texture = 1;
	}
	else
		return (printf("Not the right format for texture files !\n"), FALSE);
	return (TRUE);
}

/*
** Parse texture string that can contain bump
** Syntax: "bum:earthbump1k.xpm
*/

int	parse_bump(char *str, t_bump *bump)
{
	char	*bump_file;

	bump_file = NULL;
	if (ft_strncmp(str, "bum:", 4) == 0)
		bump_file = str + 4;
	else
    	return (printf("Not the right format for bump files !\n"), FALSE);
	if (bump_file)
	{
		bump->bump_path = ft_strdup(bump_file);
		if (!bump->bump_path)
			return (FALSE);
		bump->has_bump_map = 1;
	}
	else
		return (printf("Not the right format for bump files !\n"), FALSE);
	return (TRUE);
}
