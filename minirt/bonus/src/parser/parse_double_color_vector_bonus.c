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

/*
** Parse texture string that can contain both bump and texture maps
** Syntax: "bum:imgs/earthbump1k.xpm txm:imgs/earthmap1k.xpm" or "txm:imgs/moonmap1k.xpm"
*/
int	parse_texture(char *str, t_texture *texture, void *mlx)
{
	char	**tokens;
	char	*bump_file;
	char	*texture_file;
	char	full_path[256];
	int		i;
	
	if (!str || !texture)
		return (FALSE);
	
	// Initialize texture structure
	texture->has_texture = 0;
	texture->data = NULL;
	texture->mlx_img = NULL;
	texture->width = 0;
	texture->height = 0;
	texture->path = NULL;
	texture->has_bump_map = 0;
	texture->bump_data = NULL;
	texture->bump_mlx_img = NULL;
	texture->bump_width = 0;
	texture->bump_height = 0;
	texture->bump_path = NULL;
	
	// Check if "none" or empty string (no texture)
	if (ft_strncmp(str, "none", 4) == 0 || ft_strlen(str) == 0)
		return (TRUE);
	
	// Split by spaces to handle multiple texture types
	tokens = ft_split(str, " ");
	if (!tokens)
		return (FALSE);
	
	bump_file = NULL;
	texture_file = NULL;
	
	// Parse each token
	i = 0;
	while (tokens[i])
	{
		if (ft_strncmp(tokens[i], "bum:", 4) == 0)
			bump_file = tokens[i] + 4;  // Skip "bum:" prefix
		else if (ft_strncmp(tokens[i], "txm:", 4) == 0)
			texture_file = tokens[i] + 4;  // Skip "txm:" prefix
		else
		{
			// For backward compatibility, treat files without prefix as texture
			texture_file = tokens[i];
		}
		i++;
	}
	
	// Process texture file
	if (texture_file)
	{
		// Handle imgs/ vs images/ directory
		if (ft_strncmp(texture_file, "imgs/", 5) == 0)
		{
			ft_strlcpy(full_path, "images/", sizeof(full_path));
			ft_strlcat(full_path, texture_file + 5, sizeof(full_path));  // Skip "imgs/"
		}
		else
		{
			ft_strlcpy(full_path, "images/", sizeof(full_path));
			ft_strlcat(full_path, texture_file, sizeof(full_path));
		}
		
		if (!mlx)
		{
			texture->path = ft_strdup(full_path);
			if (!texture->path)
			{
				free_tokens(tokens);
				return (FALSE);
			}
			texture->has_texture = 1;
		}
		else
		{
			*texture = load_texture(mlx, full_path);
			if (!texture->has_texture)
				printf("Warning: Could not load texture '%s'\n", full_path);
		}
	}
	
	// Process bump file
	if (bump_file)
	{
		// Handle imgs/ vs images/ directory
		if (ft_strncmp(bump_file, "imgs/", 5) == 0)
		{
			ft_strlcpy(full_path, "images/", sizeof(full_path));
			ft_strlcat(full_path, bump_file + 5, sizeof(full_path));  // Skip "imgs/"
		}
		else
		{
			ft_strlcpy(full_path, "images/", sizeof(full_path));
			ft_strlcat(full_path, bump_file, sizeof(full_path));
		}
		
		if (!mlx)
		{
			texture->bump_path = ft_strdup(full_path);
			if (!texture->bump_path)
			{
				if (texture->path)
					free(texture->path);
				free_tokens(tokens);
				return (FALSE);
			}
			texture->has_bump_map = 1;
		}
		else
		{
			// Load bump map (we'll implement load_bump_map function)
			t_texture bump_texture = load_bump_map(mlx, full_path);
			if (bump_texture.has_texture)
			{
				texture->has_bump_map = 1;
				texture->bump_width = bump_texture.width;
				texture->bump_height = bump_texture.height;
				texture->bump_data = bump_texture.data;
				texture->bump_mlx_img = bump_texture.mlx_img;
			}
			else
				printf("Warning: Could not load bump map '%s'\n", full_path);
		}
	}
	
	free_tokens(tokens);
	return (TRUE);
}
