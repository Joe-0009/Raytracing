#include "../includes/minirt.h"
#include <stdio.h>

// Helper function to initialize default material
t_material	init_default_material(t_color3 color)
{
	t_material material;
	
	material.pattern_type = PATTERN_SOLID;
	material.color1 = color;
	material.color2 = color;
	material.scale = 1.0;
	return (material);
}

// Helper function to parse material properties from tokens
// Format: material_type color1 [color2] [scale]
int	parse_material(char **tokens, int start_idx, t_material *material, t_color3 default_color)
{
	if (!tokens[start_idx])
	{
		*material = init_default_material(default_color);
		return (TRUE);
	}
	
	if (ft_strncmp(tokens[start_idx], "solid", 5) == 0)
		material->pattern_type = PATTERN_SOLID;
	else
	{
		ft_fprintf_fd(2, "Error: Unknown pattern type '%s'\n", tokens[start_idx]);
		return (FALSE);
	}
	
	if (!tokens[start_idx + 1] || !parse_color(tokens[start_idx + 1], &material->color1))
	{
		ft_fprintf_fd(2, "Error: Missing or invalid first color for material\n");
		return (FALSE);
	}
	
	if (tokens[start_idx + 2] && tokens[start_idx + 2][0] != '\0')
	{
		if (!parse_color(tokens[start_idx + 2], &material->color2))
		{
			ft_fprintf_fd(2, "Error: Invalid second color for material\n");
			return (FALSE);
		}
	}
	else
		material->color2 = material->color1;
	
	if (tokens && tokens[start_idx + 3] && tokens[start_idx + 3][0] != '\0')
	{
		if (!parse_double(tokens[start_idx + 3], &material->scale))
		{
			ft_fprintf_fd(2, "Error: Invalid scale for material\n");
			return (FALSE);
		}
		if (material->scale <= 0.0)
		{
			ft_fprintf_fd(2, "Error: Material scale must be positive\n");
			return (FALSE);
		}
	}
	else
		material->scale = 1.0;

	return (TRUE);
}

int	parse_sphere(char **tokens, t_scene *scene)
{
	t_sphere	sphere;
	t_vec3		center;
	t_color3	color;
	double		diameter;

	// Check for basic format: sp x,y,z diameter r,g,b [material_properties]
	if (!tokens[1] || !tokens[2] || !tokens[3])
	{
		ft_fprintf_fd(2, ERR_SPHERE_FORMAT);
		ft_fprintf_fd(2, "Expected format: sp x,y,z diameter r,g,b [material_type color1 [color2] [scale]]\n");
		return (FALSE);
	}
	
	if (!parse_vector(tokens[1], &center))
	{
		return (FALSE);
	}
	if (!parse_double(tokens[2], &diameter))
	{
		return (FALSE);
	}
	if (!validate_sphere_diameter(diameter))
	{
		return (FALSE);
	}
		
	// Parse legacy color format if present
	if (tokens[3] && tokens[3][0] != '\0' && ft_strncmp(tokens[3], "solid", 5) != 0)
	{
		// Legacy format: sp x,y,z diameter r,g,b
		if (!parse_color(tokens[3], &color))
		{
			return (FALSE);
		}
		sphere.material = init_default_material(color);
		if (tokens[4])  // Extra tokens in legacy format
		{
			ft_fprintf_fd(2, ERR_SPHERE_FORMAT);
			ft_fprintf_fd(2, "Too many arguments for legacy sphere format\n");
			return (FALSE);
		}
	}
	else
	{
		// New format: sp x,y,z diameter material_type color1 [color2] [scale]
		if (!parse_material(tokens, 3, &sphere.material, (t_color3){255, 255, 255}))
		{
			return (FALSE);
		}
	}
	
	sphere.center = center;
	sphere.diameter = diameter;
	sphere.color = sphere.material.color1;  // Keep for backwards compatibility
	
	if (!add_object_to_scene(scene, SPHERE, &sphere))
	{
		return (FALSE);
	}
	return (TRUE);
}

int	parse_plane(char **tokens, t_scene *scene)
{
	t_plane		plane;
	t_vec3		point;
	t_vec3		normal;
	t_color3	color;

	// Check for basic format: pl x,y,z nx,ny,nz r,g,b [material_properties]
	if (!tokens[1] || !tokens[2] || !tokens[3])
	{
		ft_fprintf_fd(2, ERR_PLANE_FORMAT);
		ft_fprintf_fd(2, "Expected format: pl x,y,z nx,ny,nz r,g,b [material_type color1 [color2] [scale]]\n");
		return (FALSE);
	}
	
	if (!parse_vector(tokens[1], &point) || !parse_vector(tokens[2], &normal))
		return (FALSE);
	if (!validate_plane_normal(&normal))
		return (FALSE);
		
	// Parse legacy color format if present
	if (tokens[3] && tokens[3][0] != '\0' && ft_strncmp(tokens[3], "solid", 5) != 0)
	{
		// Legacy format: pl x,y,z nx,ny,nz r,g,b
		if (!parse_color(tokens[3], &color))
			return (FALSE);
		plane.material = init_default_material(color);
		if (tokens[4])  // Extra tokens in legacy format
		{
			ft_fprintf_fd(2, ERR_PLANE_FORMAT);
			ft_fprintf_fd(2, "Too many arguments for legacy plane format\n");
			return (FALSE);
		}
	}
	else
	{
		// New format: pl x,y,z nx,ny,nz material_type color1 [color2] [scale]
		if (!parse_material(tokens, 3, &plane.material, (t_color3){128, 128, 128}))
			return (FALSE);
	}
	
	plane.point = point;
	plane.normal = normal;
	plane.color = plane.material.color1;  // Keep for backwards compatibility
	
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

	// Check for minimum required tokens: cy x,y,z nx,ny,nz diameter r,g,b
	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4])
	{
		ft_fprintf_fd(2, ERR_CYLINDER_FORMAT);
		ft_fprintf_fd(2, "Expected format: cy x,y,z nx,ny,nz diameter height r,g,b [material_type color1 [color2] [scale]]\n");
		return (FALSE);
	}
	
	if (!parse_vector(tokens[1], &center))
	{
		return (FALSE);
	}
	if (!parse_vector(tokens[2], &axis))
	{
		return (FALSE);
	}
	if (!parse_double(tokens[3], &diameter))
	{
		return (FALSE);
	}
	
	// Check if we have the new format (with height) or old format (without height)
	if (tokens[5] && (ft_strchr(tokens[4], ',') == NULL)) // tokens[4] is not a color (no comma), so it's height
	{
		if (!parse_double(tokens[4], &height))
		{
			return (FALSE);
		}
		
		// Parse legacy color format if present
		if (tokens[5] && tokens[5][0] != '\0' && ft_strncmp(tokens[5], "solid", 5) != 0)
		{
			// New format: cy x,y,z nx,ny,nz diameter height r,g,b
			if (!parse_color(tokens[5], &color))
			{
				return (FALSE);
			}
			cylinder.material = init_default_material(color);
			if (tokens[6])  // Extra tokens in legacy format
			{
				ft_fprintf_fd(2, ERR_CYLINDER_FORMAT);
				ft_fprintf_fd(2, "Too many arguments for legacy cylinder format\n");
				return (FALSE);
			}
		}
		else
		{
			// New format: cy x,y,z nx,ny,nz diameter height material_type color1 [color2] [scale]
			if (!parse_material(tokens, 5, &cylinder.material, (t_color3){0, 128, 255}))
			{
				return (FALSE);
			}
		}
	}
	else
	{
		// Old format: cy x,y,z nx,ny,nz diameter r,g,b
		height = diameter; // Default height equals diameter for old format
		
		if (!tokens[4])
		{
			ft_fprintf_fd(2, ERR_CYLINDER_FORMAT);
			return (FALSE);
		}
		
		if (!parse_color(tokens[4], &color))
		{
			return (FALSE);
		}
		cylinder.material = init_default_material(color);
		if (tokens[5])  // Extra tokens in old format
		{
			ft_fprintf_fd(2, ERR_CYLINDER_FORMAT);
			ft_fprintf_fd(2, "Too many arguments for old cylinder format\n");
			return (FALSE);
		}
	}
	
	if (!validate_cylinder_dimensions(diameter, height))
	{
		return (FALSE);
	}
	
	cylinder.center = center;
	cylinder.axis = vec3_normalize(axis);
	cylinder.diameter = diameter;
	cylinder.height = height;
	cylinder.color = cylinder.material.color1;  // Keep for backwards compatibility
	
	if (!add_object_to_scene(scene, CYLINDER, &cylinder))
	{
		return (FALSE);
	}
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
