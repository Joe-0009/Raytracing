#include "../includes/minirt_app.h"
#include <stdio.h>

t_material	create_simple_material(t_color3 color)
{
	t_material	material;

	material.color = color;
	return (material);
}

int	parse_ambient(char **tokens, t_scene *scene)
{
	t_color3	color;
	double		ratio;

	if (!tokens[1] || !tokens[2])
		return (printf(ERR_AMBIENT_FORMAT),
			printf(FMT_AMBIENT_EXPECTED), FALSE);
	if (scene->has_ambient)
		return (printf(ERR_AMBIENT_ALREADY_DEFINED), FALSE);
	if (!parse_double(tokens[1], &ratio))
		return (FALSE);
	if (ratio < 0.0 || ratio > 1.0)
		return (printf(ERR_AMBIENT_RATIO_RANGE), FALSE);
	if (!parse_color(tokens[2], &color))
		return (printf(ERR_AMBIENT_COLOR_INVALID), FALSE);
	if (tokens[3])
		return (printf(ERR_AMBIENT_FORMAT),
			printf(ERR_AMBIENT_TOO_MANY_ARGS), FALSE);
	scene->ambient.ratio = ratio;
	scene->ambient.color = color;
	scene->has_ambient = TRUE;
	return (TRUE);
}

int	parse_light(char **tokens, t_scene *scene)
{
	t_vec3		position;
	t_color3	color;
	double		brightness;

	if (!tokens[1] || !tokens[2] || !tokens[3])
		return (printf(ERR_LIGHT_FORMAT),
			printf(FMT_LIGHT_EXPECTED), FALSE);
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
	if (tokens[4])
		return (printf(ERR_LIGHT_FORMAT),
			printf(ERR_LIGHT_TOO_MANY_ARGS), FALSE);
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

	if (!tokens[1] || !tokens[2] || !tokens[3])
		return (printf(ERR_SPHERE_FORMAT),
			printf(FMT_SPHERE_EXPECTED), FALSE);
	if (!parse_vector(tokens[1], &center))
		return (FALSE);
	if (!parse_double(tokens[2], &diameter))
		return (FALSE);
	if (!tokens[3] || !parse_color(tokens[3], &color))
		return (printf(ERR_SPHERE_COLOR_INVALID), FALSE);
	if (tokens[4])
		return (printf(ERR_SPHERE_FORMAT),
			printf(ERR_SPHERE_TOO_MANY_ARGS), FALSE);
	sphere.center = center;
	sphere.diameter = diameter;
	sphere.color = color;
	sphere.material = create_simple_material(color);
	if (!validate_sphere(&sphere))
		return (FALSE);
	if (!add_object_to_scene(scene, SPHERE, &sphere))
		return (FALSE);
	return (TRUE);
}

static int	parse_plane_params(char **tokens, t_vec3 *point,
			t_vec3 *normal, t_color3 *color)
{
	if (!tokens[1] || !tokens[2] || !tokens[3])
		return (printf(ERR_PLANE_FORMAT),
			printf(FMT_PLANE_EXPECTED), FALSE);
	if (!parse_vector(tokens[1], point) || !parse_vector(tokens[2], normal))
		return (FALSE);
	if (!validate_plane_normal(normal))
		return (FALSE);
	if (!tokens[3] || !parse_color(tokens[3], color))
		return (printf(ERR_PLANE_COLOR_INVALID), FALSE);
	if (tokens[4])
		return (printf(ERR_PLANE_FORMAT),
			printf(ERR_PLANE_TOO_MANY_ARGS), FALSE);
	return (TRUE);
}

int	parse_plane(char **tokens, t_scene *scene)
{
	t_plane		plane;
	t_vec3		point;
	t_vec3		normal;
	t_color3	color;

	if (!parse_plane_params(tokens, &point, &normal, &color))
		return (FALSE);
	plane.point = point;
	plane.normal = normal;
	plane.color = color;
	plane.material = create_simple_material(color);
	if (!add_object_to_scene(scene, PLANE, &plane))
		return (FALSE);
	return (TRUE);
}

static int	parse_cylinder_params(char **tokens, t_cylinder *cylinder,
			double *diameter, double *height, t_color3 *color)
{
	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4])
		return (printf(ERR_CYLINDER_FORMAT),
			printf(FMT_CYLINDER_EXPECTED), FALSE);
	if (!parse_vector(tokens[1], &cylinder->center))
		return (FALSE);
	if (!parse_vector(tokens[2], &cylinder->axis))
		return (FALSE);
	if (!parse_double(tokens[3], diameter))
		return (FALSE);
	if (tokens[5] && (ft_strchr(tokens[4], ',') == NULL))
	{
		if (!parse_double(tokens[4], height))
			return (FALSE);
		if (!tokens[5] || !parse_color(tokens[5], color))
			return (printf(ERR_CYLINDER_COLOR_INVALID), FALSE);
		if (tokens[6])
			return (printf(ERR_CYLINDER_FORMAT),
				printf(ERR_CYLINDER_TOO_MANY_ARGS), FALSE);
	}
	else
	{
		*height = *diameter;
		if (!tokens[4] || !parse_color(tokens[4], color))
			return (printf(ERR_CYLINDER_COLOR_INVALID), FALSE);
		if (tokens[5])
			return (printf(ERR_CYLINDER_FORMAT),
				printf(ERR_CYLINDER_TOO_MANY_ARGS), FALSE);
	}
	return (TRUE);
}

int	parse_cylinder(char **tokens, t_scene *scene)
{
	t_cylinder	cylinder;
	t_color3	color;
	double		diameter;
	double		height;

	if (!parse_cylinder_params(tokens, &cylinder, &diameter, &height, &color))
		return (FALSE);
	cylinder.axis = vec3_normalize(cylinder.axis);
	cylinder.diameter = diameter;
	cylinder.height = height;
	cylinder.color = color;
	cylinder.material = create_simple_material(color);
	if (!validate_cylinder(&cylinder))
		return (FALSE);
	if (!add_object_to_scene(scene, CYLINDER, &cylinder))
		return (FALSE);
	return (TRUE);
}

int	parse_cone(char **tokens, t_scene *scene)
{
	t_cone		cone;
	double		angle;
	double		height;
	t_color3	color;

	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5])
		return (printf(ERR_CONE_FORMAT),
			printf(FMT_CONE_EXPECTED), FALSE);
	if (!parse_vector(tokens[1], &cone.vertex) || !parse_vector(tokens[2], &cone.axis))
		return (FALSE);
	if (!validate_non_zero_vector(cone.axis))
		return (printf(ERR_CONE_FORMAT), FALSE);
	if (!parse_double(tokens[3], &angle) || !parse_double(tokens[4], &height))
		return (FALSE);
	if (angle > 0 && angle <= 180)
		angle = angle * M_PI / 180.0;
	if (!validate_cone_dimensions(angle, height))
		return (FALSE);
	if (!tokens[5] || !parse_color(tokens[5], &color))
		return (printf(ERR_CONE_COLOR_INVALID), FALSE);
	if (tokens[6])
		return (printf(ERR_CONE_FORMAT),
			printf(ERR_CONE_TOO_MANY_ARGS), FALSE);
	cone.axis = vec3_normalize(cone.axis);
	cone.angle = angle;
	cone.height = height;
	cone.color = color;
	cone.material = create_simple_material(color);
	if (!add_object_to_scene(scene, CONE, &cone))
		return (FALSE);
	return (TRUE);
}

int	parse_camera(char **tokens, t_scene *scene)
{
	t_vec3	position;
	t_vec3	orientation;
	double	fov;

	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		return (printf(ERR_CAMERA_FORMAT),
			printf(FMT_CAMERA_EXPECTED), FALSE);
	if (!parse_vector(tokens[1], &position) || !parse_vector(tokens[2],
			&orientation) || !parse_double(tokens[3], &fov))
		return (FALSE);
	if (!validate_non_zero_vector(orientation))
		return (printf(ERR_CAMERA_FORMAT), FALSE);
	orientation = vec3_normalize(orientation);
	if (!validate_normalized_vector(orientation))
		return (printf(ERR_CAMERA_FORMAT), FALSE);
	if (fov < 0.0 || fov > 180.0)
		return (printf(ERR_CAMERA_FORMAT),
			printf(ERR_CAMERA_FOV_RANGE), FALSE);
	scene->camera.position = position;
	scene->camera.orientation = orientation;
	scene->camera.fov = fov;
	return (TRUE);
}
