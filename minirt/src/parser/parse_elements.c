#include "../includes/minirt.h"
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
	{
		ft_fprintf_fd(2, ERR_AMBIENT_FORMAT);
		ft_fprintf_fd(2, "Expected format: A ratio r,g,b\n");
		return (FALSE);
	}
	if (scene->has_ambient)
	{
		ft_fprintf_fd(2, "Error: Ambient lighting already defined\n");
		return (FALSE);
	}
	if (!parse_double(tokens[1], &ratio))
		return (FALSE);
	if (ratio < 0.0 || ratio > 1.0)
	{
		ft_fprintf_fd(2, "Error: Ambient ratio must be between 0.0 and 1.0\n");
		return (FALSE);
	}
	if (!parse_color(tokens[2], &color))
	{
		ft_fprintf_fd(2, "Error: Invalid color for ambient lighting\n");
		return (FALSE);
	}
	if (tokens[3])
	{
		ft_fprintf_fd(2, ERR_AMBIENT_FORMAT);
		ft_fprintf_fd(2, "Too many arguments for ambient lighting\n");
		return (FALSE);
	}
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
	{
		ft_fprintf_fd(2, ERR_LIGHT_FORMAT);
		ft_fprintf_fd(2, "Expected format: L x,y,z brightness r,g,b\n");
		return (FALSE);
	}
	if (scene->has_light)
	{
		ft_fprintf_fd(2, "Error: Light source already defined\n");
		return (FALSE);
	}
	if (!parse_vector(tokens[1], &position))
		return (FALSE);
	if (!parse_double(tokens[2], &brightness))
		return (FALSE);
	if (brightness < 0.0 || brightness > 1.0)
	{
		ft_fprintf_fd(2, "Error: Light brightness must be between 0.0 and 1.0\n");
		return (FALSE);
	}
	if (!parse_color(tokens[3], &color))
	{
		ft_fprintf_fd(2, "Error: Invalid color for light source\n");
		return (FALSE);
	}
	if (tokens[4])
	{
		ft_fprintf_fd(2, ERR_LIGHT_FORMAT);
		ft_fprintf_fd(2, "Too many arguments for light source\n");
		return (FALSE);
	}
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
	{
		ft_fprintf_fd(2, ERR_SPHERE_FORMAT);
		ft_fprintf_fd(2, "Expected format: sp x,y,z diameter r,g,b\n");
		return (FALSE);
	}
	if (!parse_vector(tokens[1], &center))
		return (FALSE);
	if (!parse_double(tokens[2], &diameter))
		return (FALSE);
	if (!validate_sphere_diameter(diameter))
		return (FALSE);
	if (!tokens[3] || !parse_color(tokens[3], &color))
	{
		ft_fprintf_fd(2, "Error: Missing or invalid color for sphere\n");
		return (FALSE);
	}
	if (tokens[4])
	{
		ft_fprintf_fd(2, ERR_SPHERE_FORMAT);
		ft_fprintf_fd(2, "Too many arguments for sphere\n");
		return (FALSE);
	}
	sphere.center = center;
	sphere.diameter = diameter;
	sphere.color = color;
	sphere.material = create_simple_material(color);
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

	if (!tokens[1] || !tokens[2] || !tokens[3])
	{
		ft_fprintf_fd(2, ERR_PLANE_FORMAT);
		ft_fprintf_fd(2, "Expected format: pl x,y,z nx,ny,nz r,g,b\n");
		return (FALSE);
	}
	if (!parse_vector(tokens[1], &point) || !parse_vector(tokens[2], &normal))
		return (FALSE);
	if (!validate_plane_normal(&normal))
		return (FALSE);
	if (!tokens[3] || !parse_color(tokens[3], &color))
	{
		ft_fprintf_fd(2, "Error: Missing or invalid color for plane\n");
		return (FALSE);
	}
	if (tokens[4])
	{
		ft_fprintf_fd(2, ERR_PLANE_FORMAT);
		ft_fprintf_fd(2, "Too many arguments for plane\n");
		return (FALSE);
	}
	plane.point = point;
	plane.normal = normal;
	plane.color = color;
	plane.material = create_simple_material(color);
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

	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4])
	{
		ft_fprintf_fd(2, ERR_CYLINDER_FORMAT);
		ft_fprintf_fd(2, "Expected format: cy x,y,z nx,ny,nz diameter [height] r,g,b\n");
		return (FALSE);
	}
	if (!parse_vector(tokens[1], &center))
		return (FALSE);
	if (!parse_vector(tokens[2], &axis))
		return (FALSE);
	if (!parse_double(tokens[3], &diameter))
		return (FALSE);
	if (tokens[5] && (ft_strchr(tokens[4], ',') == NULL))
	{
		if (!parse_double(tokens[4], &height))
			return (FALSE);
		if (!tokens[5] || !parse_color(tokens[5], &color))
		{
			ft_fprintf_fd(2, "Error: Missing or invalid color for cylinder\n");
			return (FALSE);
		}
		if (tokens[6])
		{
			ft_fprintf_fd(2, ERR_CYLINDER_FORMAT);
			ft_fprintf_fd(2, "Too many arguments for cylinder\n");
			return (FALSE);
		}
	}
	else
	{
		height = diameter;
		if (!tokens[4] || !parse_color(tokens[4], &color))
		{
			ft_fprintf_fd(2, "Error: Missing or invalid color for cylinder\n");
			return (FALSE);
		}
		if (tokens[5])
		{
			ft_fprintf_fd(2, ERR_CYLINDER_FORMAT);
			ft_fprintf_fd(2, "Too many arguments for cylinder\n");
			return (FALSE);
		}
	}
	if (!validate_cylinder_dimensions(diameter, height))
		return (FALSE);
	cylinder.center = center;
	cylinder.axis = vec3_normalize(axis);
	cylinder.diameter = diameter;
	cylinder.height = height;
	cylinder.color = color;
	cylinder.material = create_simple_material(color);
	if (!add_object_to_scene(scene, CYLINDER, &cylinder))
		return (FALSE);
	return (TRUE);
}

int	parse_cone(char **tokens, t_scene *scene)
{
	t_cone		cone;
	t_vec3		vertex;
	t_vec3		axis;
	double		angle;
	double		height;
	t_color3	color;

	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5])
	{
		ft_fprintf_fd(2, ERR_CONE_FORMAT);
		ft_fprintf_fd(2, "Expected format: cn x,y,z axis_x,y,z angle height r,g,b\n");
		return (FALSE);
	}
	if (!parse_vector(tokens[1], &vertex) || !parse_vector(tokens[2], &axis))
		return (FALSE);
	if (!validate_non_zero_vector(axis))
	{
		ft_fprintf_fd(2, ERR_CONE_FORMAT);
		return (FALSE);
	}
	if (!parse_double(tokens[3], &angle) || !parse_double(tokens[4], &height))
		return (FALSE);
	if (angle > 0 && angle <= 180)
		angle = angle * M_PI / 180.0;
	if (!validate_cone_dimensions(angle, height))
		return (FALSE);
	if (!tokens[5] || !parse_color(tokens[5], &color))
	{
		ft_fprintf_fd(2, "Error: Missing or invalid color for cone\n");
		return (FALSE);
	}
	if (tokens[6])
	{
		ft_fprintf_fd(2, ERR_CONE_FORMAT);
		ft_fprintf_fd(2, "Too many arguments for cone\n");
		return (FALSE);
	}
	cone.vertex = vertex;
	cone.axis = vec3_normalize(axis);
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
	{
		ft_fprintf_fd(2, ERR_CAMERA_FORMAT);
		ft_fprintf_fd(2, "Expected format: C x,y,z nx,ny,nz fov\n");
		return (FALSE);
	}
	if (!parse_vector(tokens[1], &position) || !parse_vector(tokens[2], &orientation)
		|| !parse_double(tokens[3], &fov))
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
