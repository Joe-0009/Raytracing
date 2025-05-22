#include "../includes/minirt.h"

/**
 * Add a light to the scene
 *
 * @param scene Pointer to the scene structure
 * @param light Pointer to the light structure to add
 * @return TRUE if successful, FALSE otherwise
 */
int	add_light_to_scene(t_scene *scene, t_light *light)
{
	if (scene->num_lights >= MAX_LIGHTS)
	{
		ft_fprintf_fd(2, "Error: Maximum number of lights reached (%d)\n",
			MAX_LIGHTS);
		return (FALSE);
	}
	scene->lights[scene->num_lights] = *light;
	scene->num_lights++;
	return (TRUE);
}

/**
 * Add an object to the scene
 *
 * @param scene Pointer to the scene structure
 * @param type Object type (SPHERE, PLANE, CYLINDER)
 * @param object_data Pointer to the object data
 * @return TRUE if successful, FALSE otherwise
 */
int	add_object_to_scene(t_scene *scene, int type, void *object_data)
{
	if (scene->num_objects >= MAX_OBJECTS)
	{
		ft_fprintf_fd(2, "Error: Maximum number of objects reached (%d)\n",
			MAX_OBJECTS);
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
		ft_fprintf_fd(2, "Error: Unknown object type %d\n", type);
		return (FALSE);
	}
	scene->num_objects++;
	return (TRUE);
}
