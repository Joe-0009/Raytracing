#include "../includes/minirt_app_bonus.h"
#include <stdio.h>

int	parse_ambient(char **tokens, t_scene *scene)
{
	t_ambient	ambient;

	if (!tokens[1] || !tokens[2] || tokens[3])
		return (printf(ERR_AMBIENT_FORMAT), printf(FMT_AMBIENT_EXPECTED),
			FALSE);
	if (scene->has_ambient)
		return (printf(ERR_AMBIENT_ALREADY_DEFINED), FALSE);
	if (!parse_double(tokens[1], &ambient.ratio))
		return (FALSE);
	if (ambient.ratio < 0.0 || ambient.ratio > 1.0)
		return (printf(ERR_AMBIENT_RATIO_RANGE), FALSE);
	if (!parse_color(tokens[2], &ambient.color))
		return (printf(ERR_AMBIENT_COLOR_INVALID), FALSE);
	scene->has_ambient = TRUE;
	scene->ambient = ambient;
	return (TRUE);
}

int	parse_camera(char **tokens, t_scene *scene)
{
	t_camera	camera;

	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		return (printf(ERR_CAMERA_FORMAT), printf(FMT_CAMERA_EXPECTED), FALSE);
	if (!parse_vector(tokens[1], &camera.position) || !parse_vector(tokens[2],
			&camera.orientation) || !parse_double(tokens[3], &camera.fov))
		return (FALSE);
	if (!validate_non_zero_vector(camera.orientation))
		return (printf(ERR_CAMERA_FORMAT), FALSE);
	camera.orientation = vec3_normalize(camera.orientation);
	if (!validate_normalized_vector(camera.orientation))
		return (printf(ERR_CAMERA_FORMAT), FALSE);
	if (camera.fov < 0.0 || camera.fov > 180.0)
		return (printf(ERR_CAMERA_FORMAT), printf(ERR_CAMERA_FOV_RANGE), FALSE);
	scene->camera = camera;
	return (TRUE);
}

int	parse_light(char **tokens, t_scene *scene)
{
	t_light	light;

	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		return (printf(ERR_LIGHT_FORMAT), printf(FMT_LIGHT_EXPECTED), FALSE);
	if (scene->nbr_of_lights >= MAX_LIGHTS)
		return (printf("Error: Maximum number of lights exceeded (%d)\n",
				MAX_LIGHTS), FALSE);
	if (!parse_vector(tokens[1], &light.position))
		return (FALSE);
	if (!parse_double(tokens[2], &light.brightness))
		return (FALSE);
	if (light.brightness < 0.0 || light.brightness > 1.0)
		return (printf(ERR_LIGHT_BRIGHTNESS_RANGE), FALSE);
	if (!parse_color(tokens[3], &light.color))
		return (printf(ERR_LIGHT_COLOR_INVALID), FALSE);
	scene->light[scene->nbr_of_lights] = light;
	scene->nbr_of_lights++;
	return (TRUE);
}

int	parse_sphere(char **tokens, t_scene *scene)
{
	t_sphere	sphere;

	if (!tokens[1] || !tokens[2] || !tokens[3])
		return (printf(ERR_SPHERE_FORMAT), printf(FMT_SPHERE_EXPECTED), FALSE);
	if (!parse_vector(tokens[1], &sphere.center))
		return (FALSE);
	if (!parse_double(tokens[2], &sphere.diameter))
		return (FALSE);
	if (!parse_color(tokens[3], &sphere.color))
		return (printf(ERR_SPHERE_COLOR_INVALID), FALSE);
	sphere.rotation = vec3_create(0, 0, 0);
	if (tokens[4])
	{
		if (!parse_texture(tokens[4], &sphere.texture))
			return (FALSE);
	}
	if (tokens[5])
	{
		if (!parse_bump(tokens[5], &sphere.bump))
			return (FALSE);
	}
	if (!add_object_to_scene(scene, SPHERE, &sphere))
		return (FALSE);
	return (TRUE);
}

int	parse_plane(char **tokens, t_scene *scene)
{
	t_plane	plane;

	if (!tokens[1] || !tokens[2] || !tokens[3])
		return (printf(ERR_PLANE_FORMAT), printf(FMT_PLANE_EXPECTED), FALSE);
	if (tokens[4] && tokens[5])
		return (printf(ERR_PLANE_FORMAT), printf(FMT_PLANE_EXPECTED), FALSE);
	if (!parse_vector(tokens[1], &plane.point) || !parse_vector(tokens[2],
			&plane.normal))
		return (FALSE);
	if (!parse_color(tokens[3], &plane.color))
		return (printf(ERR_PLANE_COLOR_INVALID), FALSE);
	if (tokens[4])
	{
			return (FALSE);
	}
	if (!add_object_to_scene(scene, PLANE, &plane))
		return (FALSE);
	return (TRUE);
}
