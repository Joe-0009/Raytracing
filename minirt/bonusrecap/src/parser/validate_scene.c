#include "../includes/minirt_app.h"
#include "../includes/parser.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

static int	validate_scene_basic(t_scene *scene)
{
	if (!scene)
		return (printf(ERR_MEMORY), FALSE);
	if (scene->camera.fov == 0.0)
		return (printf(ERR_SCENE_NO_CAMERA), FALSE);
	if (!scene->has_ambient)
		return (printf(ERR_SCENE_NO_AMBIENT), FALSE);
	if (!scene->has_light)
		return (printf(ERR_SCENE_NO_LIGHT), FALSE);
	if (scene->camera.orientation.x == 0 && scene->camera.orientation.y == 0
		&& scene->camera.orientation.z == 0)
		return (printf(ERR_CAMERA_ORIENTATION_ZERO), FALSE);
	return (TRUE);
}

static int	validate_scene_objects(t_scene *scene)
{
	int	i;

	i = 0;
	while (i < scene->num_objects)
	{
		if (scene->objects[i].type == SPHERE
			&& !validate_sphere(&scene->objects[i].data.sphere))
			return (printf(ERR_SPHERE_INVALID), FALSE);
		else if (scene->objects[i].type == CYLINDER
			&& !validate_cylinder(&scene->objects[i].data.cylinder))
			return (printf(ERR_CYLINDER_INVALID), FALSE);
		else if (scene->objects[i].type == PLANE
			&& !validate_plane(&scene->objects[i].data.plane))
			return (printf(ERR_PLANE_INVALID), FALSE);
		i++;
	}
	return (TRUE);
}

int	validate_scene(t_scene *scene)
{
	if (!validate_scene_basic(scene))
		return (FALSE);
	return (validate_scene_objects(scene));
}

int	validate_scene_rendering(t_scene *scene)
{
	int	i;

	if (scene->camera.fov == 0.0)
		return (printf(ERR_SCENE_NO_CAMERA_RENDER), FALSE);
	i = 0;
	while (i < scene->num_objects)
	{
		if (scene->objects[i].type == SPHERE
			&& !validate_sphere(&scene->objects[i].data.sphere))
			return (FALSE);
		else if (scene->objects[i].type == CYLINDER
			&& !validate_cylinder(&scene->objects[i].data.cylinder))
			return (FALSE);
		else if (scene->objects[i].type == PLANE
			&& !validate_plane(&scene->objects[i].data.plane))
			return (FALSE);
		i++;
	}
	return (TRUE);
}
