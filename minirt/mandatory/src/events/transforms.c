#include "../includes/events.h"
#include "../includes/scene.h"
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
** Uses the advanced matrix transform function for better performance
*/
void	transform_update_matrix(t_transform *transform)
{
	transform->matrix = matrix4_transform(transform->translation,
			transform->rotation, transform->scale);
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
** Rotate object in scene using matrix transformations
*/
void	scene_rotate_object(t_scene *scene, int obj_index, t_vec3 rotation)
{
	t_matrix4	rotation_x;
	t_matrix4	rotation_y;
	t_matrix4	rotation_z;
	t_matrix4	combined_rotation;
	t_transform	transform;

	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;

	// Create individual rotation matrices
	rotation_x = matrix4_rotation_x(rotation.x);
	rotation_y = matrix4_rotation_y(rotation.y);
	rotation_z = matrix4_rotation_z(rotation.z);

	// Combine rotations: Z * Y * X
	combined_rotation = matrix4_multiply(rotation_z, rotation_y);
	combined_rotation = matrix4_multiply(combined_rotation, rotation_x);

	// Create transform with combined rotation matrix
	transform = transform_identity();
	transform.matrix = combined_rotation;

	// Apply transformation based on object type
	if (scene->objects[obj_index].type == SPHERE)
		transform_sphere(&scene->objects[obj_index].data.sphere, &transform);
	else if (scene->objects[obj_index].type == PLANE)
		transform_plane(&scene->objects[obj_index].data.plane, &transform);
	else if (scene->objects[obj_index].type == CYLINDER)
		transform_cylinder(&scene->objects[obj_index].data.cylinder, &transform);
}

/*
** Scale object in scene using matrix transformations
*/
void	scene_scale_object(t_scene *scene, int obj_index, double scale)
{
	t_transform	transform;

	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;

	// Create unified transform with scale
	transform = transform_identity();
	transform_scale_uniform(&transform, scale);

	// Apply transformation based on object type
	if (scene->objects[obj_index].type == SPHERE)
		transform_sphere(&scene->objects[obj_index].data.sphere, &transform);
	else if (scene->objects[obj_index].type == PLANE)
		transform_plane(&scene->objects[obj_index].data.plane, &transform);
	else if (scene->objects[obj_index].type == CYLINDER)
		transform_cylinder(&scene->objects[obj_index].data.cylinder, &transform);
}
