#include "../includes/events_bonus.h"
#include "../includes/scene_bonus.h"
#include <stdio.h>

/*
** Create identity transform
*/
t_transform	transform_identity(void)
{
	t_transform	transform;

	transform.translation = vec3_create(0, 0, 0);
	transform.rotation = vec3_create(0, 0, 0);
	transform.scale = vec3_create(1, 1, 1);
	transform.matrix = matrix4_identity();
	return (transform);
}

/*
** Update transformation matrix from translation, rotation, scale
*/
void	transform_update_matrix(t_transform *transform)
{
	t_matrix4	temp;
	t_matrix4	rotation_combined;

	temp = matrix4_scale(transform->scale);
	rotation_combined = matrix4_multiply(
			matrix4_rotation_x(transform->rotation.x), temp);
	rotation_combined = matrix4_multiply(
			matrix4_rotation_y(transform->rotation.y), rotation_combined);
	rotation_combined = matrix4_multiply(
			matrix4_rotation_z(transform->rotation.z), rotation_combined);
	transform->matrix = matrix4_multiply(
			matrix4_translation(transform->translation), rotation_combined);
}

/*
** Translate object in scene
*/
void	scene_translate_object(t_scene *scene, int obj_index, t_vec3 delta)
{
	t_transform	transform;

	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;
	transform = transform_identity();
	transform_translate(&transform, delta);
	if (scene->objects[obj_index].type == SPHERE)
		transform_sphere(&scene->objects[obj_index].data.sphere, &transform);
	else if (scene->objects[obj_index].type == PLANE)
		transform_plane(&scene->objects[obj_index].data.plane, &transform);
	else if (scene->objects[obj_index].type == CYLINDER)
		transform_cylinder(&scene->objects[obj_index].data.cylinder,
			&transform);
}

/*
** Rotate object in scene around its own center
*/
void	scene_rotate_object(t_scene *scene, int obj_index, t_vec3 rotation)
{
	t_vec3	axis;
	double	angle;

	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;
	angle = vec3_length(rotation);
	if (angle < 0.0001)
		return ;
	axis = vec3_normalize(rotation);
	if (scene->objects[obj_index].type == SPHERE)
		return ;
	else if (scene->objects[obj_index].type == PLANE)
	{
		scene->objects[obj_index].data.plane.normal = vec3_rotate_around_axis(
				scene->objects[obj_index].data.plane.normal, axis, angle);
		scene->objects[obj_index].data.plane.normal = vec3_normalize(
				scene->objects[obj_index].data.plane.normal);
	}
	else if (scene->objects[obj_index].type == CYLINDER)
	{
		scene->objects[obj_index].data.cylinder.axis = vec3_rotate_around_axis(
				scene->objects[obj_index].data.cylinder.axis, axis, angle);
		scene->objects[obj_index].data.cylinder.axis = vec3_normalize(
				scene->objects[obj_index].data.cylinder.axis);
	}
}

/*
** Scale object in scene
*/
void	scene_scale_object(t_scene *scene, int obj_index, double scale)
{
	t_transform	transform;

	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;
	transform = transform_identity();
	transform_scale_uniform(&transform, scale);
	if (scene->objects[obj_index].type == SPHERE)
		transform_sphere(&scene->objects[obj_index].data.sphere, &transform);
	else if (scene->objects[obj_index].type == CYLINDER)
		transform_cylinder(&scene->objects[obj_index].data.cylinder,
			&transform);
}
