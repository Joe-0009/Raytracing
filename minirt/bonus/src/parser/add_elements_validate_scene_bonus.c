#include "../includes/minirt_app_bonus.h"
#include "../includes/parser_bonus.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

int	add_object_to_scene(t_scene *scene, int type, void *object_data)
{
	if (scene->num_objects >= MAX_OBJECTS)
	{
		printf("Error: Maximum number of objects reached (%d)\n", MAX_OBJECTS);
		return (FALSE);
	}
	scene->objects[scene->num_objects].type = type;
	if (type == SPHERE)
		scene->objects[scene->num_objects].data.sphere
			= *(t_sphere *)object_data;
	else if (type == PLANE)
		scene->objects[scene->num_objects].data.plane
			= *(t_plane *)object_data;
	else if (type == CYLINDER)
		scene->objects[scene->num_objects].data.cylinder
			= *(t_cylinder *)object_data;
	else if (type == CONE)
		scene->objects[scene->num_objects].data.cone
			= *(t_cone *)object_data;
	else
	{
		printf("Error: Unknown object type %d\n", type);
		return (FALSE);
	}
	scene->num_objects++;
	return (TRUE);
}

static int	validate_scene_basic(t_scene *scene)
{
	if (!scene)
		return (printf(ERR_MEMORY), FALSE);
	if (scene->camera.fov == 0.0)
		return (printf(ERR_SCENE_NO_CAMERA), FALSE);
	if (!scene->has_ambient)
		return (printf(ERR_SCENE_NO_AMBIENT), FALSE);
	if (scene->nbr_of_lights == 0)
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
			return (FALSE);
		else if (scene->objects[i].type == CYLINDER
			&& !validate_cylinder(&scene->objects[i].data.cylinder))
			return (FALSE);
		else if (scene->objects[i].type == PLANE
			&& !validate_plane(&scene->objects[i].data.plane))
			return (FALSE);
		else if (scene->objects[i].type == CONE
			&& !validate_cone(&scene->objects[i].data.cone))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	validate_scene(t_scene *scene)
{
	if (!validate_scene_basic(scene))
	{
		return (FALSE);
	}
	if (!validate_scene_objects(scene))
	{
		return (FALSE);
	}
	return (TRUE);
}
