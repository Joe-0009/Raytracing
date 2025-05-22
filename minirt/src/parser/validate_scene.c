#include "../includes/minirt.h"
#include "../includes/parser.h"
#include <math.h>
#include <string.h>

#define ERR_FILE_FORMAT "Error: File must have .rt extension\n"

int	validate_scene(t_scene *scene)
{
	if (!scene)
	{
		ft_fprintf_fd(2, ERR_MEMORY);
		return (FALSE);
	}
	if (scene->camera.fov == 0.0)
	{
		ft_fprintf_fd(2, "Error: Camera not defined\n");
		return (FALSE);
	}
	if (scene->ambient.ratio == 0.0)
	{
		ft_fprintf_fd(2, "Error: Ambient light not defined\n");
		return (FALSE);
	}
	if (scene->ambient.ratio < 0.0 || scene->ambient.ratio > 1.0)
	{
		ft_fprintf_fd(2, "Error: Ambient light ratio must be between 0.0 and 1.0\n");
		return (FALSE);
	}
	if (scene->num_lights == 0)
	{
		ft_fprintf_fd(2, "Error: No lights defined\n");
		return (FALSE);
	}
	if (scene->camera.orientation.x == 0 && scene->camera.orientation.y == 0 && scene->camera.orientation.z == 0)
	{
		ft_fprintf_fd(2, "Error: Camera orientation vector cannot be (0,0,0)\n");
		return (FALSE);
	}
	int i = 0;
	while (i < scene->num_objects)
	{
		if (scene->objects[i].type == SPHERE && !validate_sphere(&scene->objects[i].data.sphere))
		{
			ft_fprintf_fd(2, "Error: Invalid sphere\n");
			return (FALSE);
		}
		else if (scene->objects[i].type == CYLINDER && !validate_cylinder(&scene->objects[i].data.cylinder))
		{
			ft_fprintf_fd(2, "Error: Invalid cylinder\n");
			return (FALSE);
		}
		else if (scene->objects[i].type == PLANE && !validate_plane(&scene->objects[i].data.plane))
		{
			ft_fprintf_fd(2, "Error: Invalid plane\n");
			return (FALSE);
		}
		i++;
	}
	return (TRUE);
}

int	validate_scene_rendering(t_scene *scene)
{
	int	i;

	if (scene->camera.fov == 0.0)
	{
		ft_fprintf_fd(2, "Error: No camera in scene\n");
		return (FALSE);
	}
	if (scene->ambient.ratio == 0.0)
	{
		ft_fprintf_fd(2, "Error: No ambient light in scene\n");
		return (FALSE);
	}
	if (scene->num_lights == 0)
	{
		ft_fprintf_fd(2, "Error: No lights in scene\n");
		return (FALSE);
	}
	i = 0;
	while (i < scene->num_objects)
	{
		if (scene->objects[i].type == SPHERE && !validate_sphere(&scene->objects[i].data.sphere))
			return (FALSE);
		else if (scene->objects[i].type == CYLINDER && !validate_cylinder(&scene->objects[i].data.cylinder))
			return (FALSE);
		else if (scene->objects[i].type == PLANE && !validate_plane(&scene->objects[i].data.plane))
			return (FALSE);
		i++;
	}
	i = 0;
	while (i < scene->num_lights)
	{
		if (!validate_light(&scene->lights[i]))
			return (FALSE);
		i++;
	}
	return (TRUE);
}
