#include "../includes/minirt.h"

int	parse_light(char **tokens, t_scene *scene)
{
	t_light		light;
	t_vec3		position;
	t_color3	color;
	double		brightness;

	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
	{
		ft_fprintf_fd(2, ERR_LIGHT_FORMAT);
		ft_fprintf_fd(2, "Expected format: L x,y,z brightness r,g,b\n");
		return (FALSE);
	}
	if (!parse_vector(tokens[1], &position) || !parse_double(tokens[2],
			&brightness) || !parse_color(tokens[3], &color))
		return (FALSE);
	if (!validate_light_brightness(brightness))
		return (FALSE);
	light.position = position;
	light.brightness = brightness;
	light.color = color;
	if (!add_light_to_scene(scene, &light))
		return (FALSE);
	return (TRUE);
}

int	parse_sphere(char **tokens, t_scene *scene)
{
	t_sphere	sphere;
	t_vec3		center;
	t_color3	color;
	double		diameter;

	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
	{
		ft_fprintf_fd(2, ERR_SPHERE_FORMAT);
		ft_fprintf_fd(2, "Expected format: sp x,y,z diameter r,g,b\n");
		return (FALSE);
	}
	if (!parse_vector(tokens[1], &center) || !parse_double(tokens[2],
			&diameter) || !parse_color(tokens[3], &color))
		return (FALSE);
	if (!validate_sphere_diameter(diameter))
		return (FALSE);
	sphere.center = center;
	sphere.diameter = diameter;
	sphere.color = color;
	if (!add_object_to_scene(scene, SPHERE, &sphere))
		return (FALSE);
	return (TRUE);
}

int	parse_plane(char **tokens, t_scene *scene)
{
	t_plane		plane;
	t_vec3		point;
	t_vec3		normal;
	t_color3	color;

	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
	{
		ft_fprintf_fd(2, ERR_PLANE_FORMAT);
		ft_fprintf_fd(2, "Expected format: pl x,y,z nx,ny,nz r,g,b\n");
		return (FALSE);
	}
	if (!parse_vector(tokens[1], &point) || !parse_vector(tokens[2], &normal)
		|| !parse_color(tokens[3], &color))
		return (FALSE);
	if (!validate_plane_normal(&normal))
		return (FALSE);
	plane.point = point;
	plane.normal = normal;
	plane.color = color;
	if (!add_object_to_scene(scene, PLANE, &plane))
		return (FALSE);
	return (TRUE);
}

int	parse_cylinder(char **tokens, t_scene *scene)
{
	t_cylinder	cylinder;
	t_vec3		center;
	t_vec3		axis;
	t_color3	color;
	double		diameter;
	double		height;

	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5]
		|| tokens[6])
	{
		ft_fprintf_fd(2, ERR_CYLINDER_FORMAT);
		ft_fprintf_fd(2,
			"Expected format: cy x,y,z nx,ny,nz diameter height r,g,b\n");
		return (FALSE);
	}
	if (!parse_vector(tokens[1], &center) || !parse_vector(tokens[2], &axis)
		|| !parse_double(tokens[3], &diameter)
		|| !parse_double(tokens[4], &height) || !parse_color(tokens[5],
			&color))
		return (FALSE);
	if (!validate_cylinder_dimensions(diameter, height))
		return (FALSE);
	cylinder.center = center;
	cylinder.axis = vec3_normalize(axis);
	cylinder.diameter = diameter;
	cylinder.height = height;
	cylinder.color = color;
	if (!add_object_to_scene(scene, CYLINDER, &cylinder))
		return (FALSE);
	return (TRUE);
}

int parse_ambient(char **tokens, t_scene *scene)
{
    double ratio;
    t_color3 color;

    if (!tokens[1] || !tokens[2] || tokens[3])
    {
        ft_fprintf_fd(2, ERR_AMBIENT_FORMAT);
        ft_fprintf_fd(2, "Expected format: A ratio r,g,b\n");
        return (FALSE);
    }
    if (!parse_double(tokens[1], &ratio) || !parse_color(tokens[2], &color))
        return (FALSE);
    if (ratio < 0.0 || ratio > 1.0)
    {
        ft_fprintf_fd(2, ERR_AMBIENT_FORMAT);
        ft_fprintf_fd(2, "Ambient light ratio must be between 0.0 and 1.0\n");
        return (FALSE);
    }
    scene->ambient.ratio = ratio;
    scene->ambient.color = color;
    return (TRUE);
}

int parse_camera(char **tokens, t_scene *scene)
{
    t_vec3 position;
    t_vec3 orientation;
    double fov;

    if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
    {
        ft_fprintf_fd(2, ERR_CAMERA_FORMAT);
        ft_fprintf_fd(2, "Expected format: C x,y,z nx,ny,nz fov\n");
        return (FALSE);
    }
    if (!parse_vector(tokens[1], &position) || !parse_vector(tokens[2], &orientation) || !parse_double(tokens[3], &fov))
        return (FALSE);
    if (!validate_non_zero_vector(orientation))
    {
        ft_fprintf_fd(2, ERR_CAMERA_FORMAT);
        return (FALSE);
    }
    orientation = vec3_normalize(orientation);
    if (!validate_normalized_vector(orientation))
    {
        ft_fprintf_fd(2, ERR_CAMERA_FORMAT);
        return (FALSE);
    }
    if (fov < 0.0 || fov > 180.0)
    {
        ft_fprintf_fd(2, ERR_CAMERA_FORMAT);
        ft_fprintf_fd(2, "Camera FOV must be between 0 and 180 degrees\n");
        return (FALSE);
    }
    scene->camera.position = position;
    scene->camera.orientation = orientation;
    scene->camera.fov = fov;
    return (TRUE);
}
