#include "../includes/minirt_app.h"
#include "../includes/parser.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

int	parse_cylinder(char **tokens, t_scene *scene)
{
	t_cylinder	cylinder;

	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5])
		return (printf(ERR_CYLINDER_FORMAT), printf(FMT_CYLINDER_EXPECTED),
			FALSE);
	if (!parse_vector(tokens[1], &cylinder.center))
		return (FALSE);
	if (!parse_vector(tokens[2], &cylinder.axis))
		return (FALSE);
	if (!parse_double(tokens[3], &cylinder.diameter))
		return (FALSE);
	if (!parse_double(tokens[4], &cylinder.height))
		return (FALSE);
	if (!parse_color(tokens[5], &cylinder.color))
		return (printf(ERR_CYLINDER_COLOR_INVALID), FALSE);
	if (tokens[6])
		return (printf(ERR_CYLINDER_FORMAT), printf(FMT_CYLINDER_EXPECTED),
			FALSE);
	cylinder.axis = vec3_normalize(cylinder.axis);
	if (!add_object_to_scene(scene, CYLINDER, &cylinder))
		return (FALSE);
	return (TRUE);
}

int	add_object_to_scene(t_scene *scene, int type, void *object_data)
{
	if (scene->num_objects >= MAX_OBJECTS)
	{
		printf("Error: Maximum number of objects reached (%d)\n", MAX_OBJECTS);
		return (FALSE);
	}
	scene->objects[scene->num_objects].type = type;
	if (type == SPHERE)
		scene->objects[scene->num_objects].data.sphere = *(t_sphere *)object_data;
	else if (type == PLANE)
		scene->objects[scene->num_objects].data.plane = *(t_plane *)object_data;
	else if (type == CYLINDER)
		scene->objects[scene->num_objects].data.cylinder = *(t_cylinder *)object_data;
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

int	validate_scene(t_scene *scene)
{
	if (!validate_scene_basic(scene))
		return (FALSE);
	return (validate_scene_objects(scene));
}
