#include "../includes/minirt_app_bonus.h"
#include "../includes/scene_bonus.h"
#include <math.h>

int	parse_cylinder(char **tokens, t_scene *scene)
{
	t_cylinder	cylinder;

	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5])
		return (printf(ERR_CYLINDER_FORMAT), printf(FMT_CYLINDER_EXPECTED),
			FALSE);
	if (tokens[6] && tokens[7])
		return (printf(ERR_CYLINDER_FORMAT), printf(FMT_CYLINDER_EXPECTED),
			FALSE);
	if (!parse_vector(tokens[1], &cylinder.center))
		return (FALSE);
	if (!parse_vector(tokens[2], &cylinder.axis))
		return (FALSE);
	if (!parse_double(tokens[3], &cylinder.diameter))
		return (FALSE);
	if (!parse_double(tokens[4], &cylinder.height))
		return (FALSE);
	if (!parse_color(tokens[5], &cylinder.color))
		return (printf(ERR_CYLINDER_COLOR_INVALID), FALSE);
	
	// Initialize texture as no texture
	cylinder.texture.has_texture = 0;
	cylinder.texture.data = NULL;
	cylinder.texture.mlx_img = NULL;
	
	// Parse optional texture parameter
	if (tokens[6])
	{
		if (!parse_texture(tokens[6], &cylinder.texture, NULL))
			return (FALSE);
	}
	
	cylinder.axis = vec3_normalize(cylinder.axis);
	if (!add_object_to_scene(scene, CYLINDER, &cylinder))
		return (FALSE);
	return (TRUE);
}

int	parse_cone(char **tokens, t_scene *scene)
{
	t_cone cone;

	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5])
		return (printf(ERR_CONE_FORMAT), printf(FMT_CONE_EXPECTED), FALSE);
	if (tokens[6] && tokens[7])
		return (printf(ERR_CONE_FORMAT), printf(FMT_CONE_EXPECTED), FALSE);
	if (!parse_vector(tokens[1], &cone.vertex))
		return (FALSE);
	if (!parse_vector(tokens[2], &cone.axis))
		return (FALSE);
	if (!parse_double(tokens[3], &cone.angle))
		return (FALSE);
	if (cone.angle > 0 && cone.angle <= 180)
		cone.angle = cone.angle * M_PI / 180.0;
	if (!parse_double(tokens[4], &cone.height))
		return (FALSE);
	if (!parse_color(tokens[5], &cone.color))
		return (FALSE);
	
	// Initialize texture as no texture
	cone.texture.has_texture = 0;
	cone.texture.data = NULL;
	cone.texture.mlx_img = NULL;
	
	// Parse optional texture parameter
	if (tokens[6])
	{
		if (!parse_texture(tokens[6], &cone.texture, NULL))
			return (FALSE);
	}
	
	cone.axis = vec3_normalize(cone.axis);
	if (!add_object_to_scene(scene, CONE, &cone))
		return (FALSE);
	return (TRUE);
}
