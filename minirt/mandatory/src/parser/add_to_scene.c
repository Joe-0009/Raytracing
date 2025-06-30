#include "../includes/minirt_app.h"

int	add_object_to_scene(t_scene *scene, int type, void *object_data)
{
	if (scene->num_objects >= MAX_OBJECTS)
	{
		printf("Error: Maximum number of objects reached (%d)\n",
			MAX_OBJECTS);
		return (FALSE);
	}
	scene->objects[scene->num_objects].type = type;
	if (type == SPHERE)
		scene->objects[scene->num_objects].data.sphere = *(t_sphere *)object_data;
	else if (type == PLANE)
		scene->objects[scene->num_objects].data.plane = *(t_plane *)object_data;
	else if (type == CYLINDER)
		scene->objects[scene->num_objects].data.cylinder =
			*(t_cylinder *)object_data;
	else if (type == CONE)
		scene->objects[scene->num_objects].data.cone = *(t_cone *)object_data;
	else
	{
		printf("Error: Unknown object type %d\n", type);
		return (FALSE);
	}
	scene->num_objects++;
	return (TRUE);
}
