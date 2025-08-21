#include "../includes/events.h"
#include "../includes/scene.h"
#include <math.h>
#include <stdio.h>

/*
** Translate object in scene
*/
void	scene_translate_object(t_scene *scene, int obj_index, t_vec3 delta)
{
	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;
	if (scene->objects[obj_index].type == SPHERE)
		scene->objects[obj_index].data.sphere.center = vec3_add
			(scene->objects[obj_index].data.sphere.center, delta);
	else if (scene->objects[obj_index].type == PLANE)
		scene->objects[obj_index].data.plane.point = vec3_add
			(scene->objects[obj_index].data.plane.point, delta);
	else if (scene->objects[obj_index].type == CYLINDER)
		scene->objects[obj_index].data.cylinder.center = vec3_add
			(scene->objects[obj_index].data.cylinder.center, delta);
}

/*
** Rotate object in scene
*/
void	scene_rotate_object(t_scene *scene, int obj_index, t_vec3 rotation)
{
	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;
	if (scene->objects[obj_index].type == PLANE)
		scene->objects[obj_index].data.plane.normal = vec3_rotate
			(scene->objects[obj_index].data.plane.normal, rotation);
	else if (scene->objects[obj_index].type == CYLINDER)
		scene->objects[obj_index].data.cylinder.axis = vec3_rotate
			(scene->objects[obj_index].data.cylinder.axis, rotation);
}

/*
** Scale object in scene
*/
void	scene_scale_object(t_scene *scene, int obj_index, double scale)
{
	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;
	if (scene->objects[obj_index].type == SPHERE)
		scene->objects[obj_index].data.sphere.diameter *= scale;
	else if (scene->objects[obj_index].type == CYLINDER)
	{
		scene->objects[obj_index].data.cylinder.diameter *= scale;
		scene->objects[obj_index].data.cylinder.height *= scale;
	}
}
