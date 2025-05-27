#include "../includes/minirt.h"
#include "../includes/parser.h"
#include <math.h>

static int	validate_camera(t_scene *scene)
{
	if (scene->camera.fov == 0.0)
	{
		ft_fprintf_fd(2, "Error: Camera not defined\n");
		return (FALSE);
	}
	if (scene->camera.orientation.x == 0 && scene->camera.orientation.y == 0
		&& scene->camera.orientation.z == 0)
	{
		ft_fprintf_fd(2, "Error: Camera orientation vector cannot be (0,0,0)\n");
		return (FALSE);
	}
	return (TRUE);
}

static int	validate_object(t_object *obj)
{
	if (obj->type == SPHERE)
		return (validate_sphere(&obj->data.sphere));
	else if (obj->type == CYLINDER)
		return (validate_cylinder(&obj->data.cylinder));
	else if (obj->type == PLANE)
		return (validate_plane(&obj->data.plane));
	return (TRUE);
}

int	validate_scene(t_scene *scene)
{
	int	i;

	if (!scene)
		return (ft_fprintf_fd(2, ERR_MEMORY), FALSE);
	if (!validate_camera(scene))
		return (FALSE);
	i = 0;
	while (i < scene->num_objects)
	{
		if (!validate_object(&scene->objects[i]))
			return (ft_fprintf_fd(2, "Error: Invalid object\n"), FALSE);
		i++;
	}
	return (TRUE);
}

int	validate_scene_rendering(t_scene *scene)
{
	int	i;

	if (!validate_camera(scene))
		return (FALSE);
	i = 0;
	while (i < scene->num_objects)
	{
		if (!validate_object(&scene->objects[i]))
			return (FALSE);
		i++;
	}
	return (TRUE);
}
