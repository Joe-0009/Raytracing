#include "../includes/minirt_app_bonus.h"
#include <stdio.h>

int	parse_ambient(char **tokens, t_scene *scene)
{
	t_color3	color;
	double		ratio;

	if (!tokens[1] || !tokens[2] || tokens[3])
		return (printf(ERR_AMBIENT_FORMAT), printf(FMT_AMBIENT_EXPECTED),
			FALSE);
	if (scene->has_ambient)
		return (printf(ERR_AMBIENT_ALREADY_DEFINED), FALSE);
	if (!parse_double(tokens[1], &ratio))
		return (FALSE);
	if (ratio < 0.0 || ratio > 1.0)
		return (printf(ERR_AMBIENT_RATIO_RANGE), FALSE);
	if (!parse_color(tokens[2], &color))
		return (printf(ERR_AMBIENT_COLOR_INVALID), FALSE);
	scene->ambient.ratio = ratio;
	scene->ambient.color = color;
	scene->has_ambient = TRUE;
	return (TRUE);
}

int	parse_camera(char **tokens, t_scene *scene)
{
	t_vec3	position;
	t_vec3	orientation;
	double	fov;

	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		return (printf(ERR_CAMERA_FORMAT), printf(FMT_CAMERA_EXPECTED), FALSE);
	if (!parse_vector(tokens[1], &position) || !parse_vector(tokens[2],
			&orientation) || !parse_double(tokens[3], &fov))
		return (FALSE);
	if (!validate_non_zero_vector(orientation))
		return (printf(ERR_CAMERA_FORMAT), FALSE);
	orientation = vec3_normalize(orientation);
	if (!validate_normalized_vector(orientation))
		return (printf(ERR_CAMERA_FORMAT), FALSE);
	if (fov < 0.0 || fov > 180.0)
		return (printf(ERR_CAMERA_FORMAT), printf(ERR_CAMERA_FOV_RANGE), FALSE);
	scene->camera.position = position;
	scene->camera.orientation = orientation;
	scene->camera.fov = fov;
	return (TRUE);
}

int	parse_light(char **tokens, t_scene *scene)
{
	t_vec3		position;
	t_color3	color;
	double		brightness;

	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		return (printf(ERR_LIGHT_FORMAT), printf(FMT_LIGHT_EXPECTED), FALSE);
	if (scene->has_light)
		return (printf(ERR_LIGHT_ALREADY_DEFINED), FALSE);
	if (!parse_vector(tokens[1], &position))
		return (FALSE);
	if (!parse_double(tokens[2], &brightness))
		return (FALSE);
	if (brightness < 0.0 || brightness > 1.0)
		return (printf(ERR_LIGHT_BRIGHTNESS_RANGE), FALSE);
	if (!parse_color(tokens[3], &color))
		return (printf(ERR_LIGHT_COLOR_INVALID), FALSE);
	scene->light.position = position;
	scene->light.brightness = brightness;
	scene->light.color = color;
	scene->has_light = TRUE;
	return (TRUE);
}

int	parse_sphere(char **tokens, t_scene *scene)
{
	t_sphere	sphere;
	t_vec3		center;
	t_color3	color;
	double		diameter;

	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		return (printf(ERR_SPHERE_FORMAT), printf(FMT_SPHERE_EXPECTED), FALSE);
	if (!parse_vector(tokens[1], &center))
		return (FALSE);
	if (!parse_double(tokens[2], &diameter))
		return (FALSE);
	if (!parse_color(tokens[3], &color))
		return (printf(ERR_SPHERE_COLOR_INVALID), FALSE);
	sphere.center = center;
	sphere.diameter = diameter;
	sphere.color = color;
	if (!add_object_to_scene(scene, SPHERE, &sphere))
		return (FALSE);
	return (TRUE);
}

int	parse_plane(char **tokens, t_scene *scene)
{
	t_plane	plane;

	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		return (printf(ERR_PLANE_FORMAT), printf(FMT_PLANE_EXPECTED), FALSE);
	if (!parse_vector(tokens[1], &plane.point) || !parse_vector(tokens[2],
			&plane.normal))
		return (FALSE);
	if (!validate_plane_normal(&plane.normal))
		return (FALSE);
	if (!parse_color(tokens[3], &plane.color))
		return (printf(ERR_PLANE_COLOR_INVALID), FALSE);
	if (!add_object_to_scene(scene, PLANE, &plane))
		return (FALSE);
	return (TRUE);
}
