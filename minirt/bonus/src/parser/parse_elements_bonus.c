#include "../includes/minirt_app_bonus.h"
#include <stdio.h>

int	parse_ambient(char **tokens, t_scene *scene)
{
	t_ambient	ambient;

	printf("Debug: parse_ambient called\n");
	
	if (!tokens[1] || !tokens[2] || tokens[3])
	{
		printf("Debug: parse_ambient - invalid token count\n");
		return (printf(ERR_AMBIENT_FORMAT), printf(FMT_AMBIENT_EXPECTED),
			FALSE);
	}
	if (scene->has_ambient)
	{
		printf("Debug: parse_ambient - already defined\n");
		return (printf(ERR_AMBIENT_ALREADY_DEFINED), FALSE);
	}
	if (!parse_double(tokens[1], &ambient.ratio))
	{
		printf("Debug: parse_ambient - failed to parse ratio\n");
		return (FALSE);
	}
	if (ambient.ratio < 0.0 || ambient.ratio > 1.0)
	{
		printf("Debug: parse_ambient - ratio out of range: %f\n", ambient.ratio);
		return (printf(ERR_AMBIENT_RATIO_RANGE), FALSE);
	}
	if (!parse_color(tokens[2], &ambient.color))
	{
		printf("Debug: parse_ambient - failed to parse color\n");
		return (printf(ERR_AMBIENT_COLOR_INVALID), FALSE);
	}
	scene->has_ambient = TRUE;
	scene->ambient = ambient;
	printf("Debug: parse_ambient - success! ratio=%f, color=(%f,%f,%f)\n", 
		ambient.ratio, ambient.color.x, ambient.color.y, ambient.color.z);
	return (TRUE);
}

int	parse_camera(char **tokens, t_scene *scene)
{
	t_camera	camera;

	printf("Debug: parse_camera called\n");
	
	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
	{
		printf("Debug: parse_camera - invalid token count\n");
		return (printf(ERR_CAMERA_FORMAT), printf(FMT_CAMERA_EXPECTED), FALSE);
	}
	if (!parse_vector(tokens[1], &camera.position) || !parse_vector(tokens[2],
			&camera.orientation) || !parse_double(tokens[3], &camera.fov))
	{
		printf("Debug: parse_camera - failed to parse position/orientation/fov\n");
		return (FALSE);
	}
	if (!validate_non_zero_vector(camera.orientation))
	{
		printf("Debug: parse_camera - orientation is zero vector\n");
		return (printf(ERR_CAMERA_FORMAT), FALSE);
	}
	camera.orientation = vec3_normalize(camera.orientation);
	if (!validate_normalized_vector(camera.orientation))
	{
		printf("Debug: parse_camera - failed to normalize orientation\n");
		return (printf(ERR_CAMERA_FORMAT), FALSE);
	}
	if (camera.fov < 0.0 || camera.fov > 180.0)
	{
		printf("Debug: parse_camera - fov out of range: %f\n", camera.fov);
		return (printf(ERR_CAMERA_FORMAT), printf(ERR_CAMERA_FOV_RANGE), FALSE);
	}
	scene->camera = camera;
	printf("Debug: parse_camera - success! fov=%f, pos=(%f,%f,%f), ori=(%f,%f,%f)\n",
		camera.fov, camera.position.x, camera.position.y, camera.position.z,
		camera.orientation.x, camera.orientation.y, camera.orientation.z);
	return (TRUE);
}

int	parse_light(char **tokens, t_scene *scene)
{
	t_light	light;

	printf("Debug: parse_light called\n");
	
	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
	{
		printf("Debug: parse_light - invalid token count\n");
		return (printf(ERR_LIGHT_FORMAT), printf(FMT_LIGHT_EXPECTED), FALSE);
	}
	if (scene->nbr_of_lights >= MAX_LIGHTS)
	{
		printf("Debug: parse_light - max lights exceeded\n");
		return (printf("Error: Maximum number of lights exceeded (%d)\n",
				MAX_LIGHTS), FALSE);
	}
	if (!parse_vector(tokens[1], &light.position))
	{
		printf("Debug: parse_light - failed to parse position\n");
		return (FALSE);
	}
	if (!parse_double(tokens[2], &light.brightness))
	{
		printf("Debug: parse_light - failed to parse brightness\n");
		return (FALSE);
	}
	if (light.brightness < 0.0 || light.brightness > 1.0)
	{
		printf("Debug: parse_light - brightness out of range: %f\n", light.brightness);
		return (printf(ERR_LIGHT_BRIGHTNESS_RANGE), FALSE);
	}
	if (!parse_color(tokens[3], &light.color))
	{
		printf("Debug: parse_light - failed to parse color\n");
		return (printf(ERR_LIGHT_COLOR_INVALID), FALSE);
	}
	scene->light[scene->nbr_of_lights] = light;
	scene->nbr_of_lights++;
	printf("Debug: parse_light - success! brightness=%f, pos=(%f,%f,%f), color=(%f,%f,%f)\n",
		light.brightness, light.position.x, light.position.y, light.position.z,
		light.color.x, light.color.y, light.color.z);
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
	
	// Initialize texture as no texture
	sphere.texture.has_texture = 0;
	sphere.texture.data = NULL;
	sphere.texture.mlx_img = NULL;
	sphere.texture.path = NULL;
	sphere.texture.has_bump_map = 0;
	sphere.texture.bump_data = NULL;
	sphere.texture.bump_mlx_img = NULL;
	sphere.texture.bump_path = NULL;
	
	// Initialize rotation
	sphere.rotation = vec3_create(0, 0, 0);
	
	// Parse optional texture parameters (can be multiple)
	if (tokens[4])
	{
		char	texture_string[512];
		int		i;
		
		printf("Debug: Found texture token: %s\n", tokens[4]);
		
		texture_string[0] = '\0';
		i = 4;
		
		// Concatenate all texture tokens with spaces
		while (tokens[i])
		{
			if (i > 4)
				ft_strlcat(texture_string, " ", sizeof(texture_string));
			ft_strlcat(texture_string, tokens[i], sizeof(texture_string));
			i++;
		}
		
		printf("Debug: Full texture string: '%s'\n", texture_string);
		
		if (!parse_texture(texture_string, &sphere.texture, NULL))
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
	
	// Initialize texture as no texture
	plane.texture.has_texture = 0;
	plane.texture.data = NULL;
	plane.texture.mlx_img = NULL;
	plane.texture.path = NULL;
	
	// Parse optional texture parameter
	if (tokens[4])
	{
		if (!parse_texture(tokens[4], &plane.texture, NULL))
			return (FALSE);
	}
	
	if (!add_object_to_scene(scene, PLANE, &plane))
		return (FALSE);
	return (TRUE);
}
