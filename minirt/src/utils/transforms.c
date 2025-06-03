#include "../includes/transforms.h"
#include "../includes/math_utils.h"
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
	t_matrix4	translation_matrix;
	t_matrix4	rotation_x_matrix;
	t_matrix4	rotation_y_matrix;
	t_matrix4	rotation_z_matrix;
	t_matrix4	scale_matrix;
	t_matrix4	temp;

	translation_matrix = matrix4_translation(transform->translation);
	rotation_x_matrix = matrix4_rotation_x(transform->rotation.x);
	rotation_y_matrix = matrix4_rotation_y(transform->rotation.y);
	rotation_z_matrix = matrix4_rotation_z(transform->rotation.z);
	scale_matrix = matrix4_scale(transform->scale);
	/* ** Apply transformations: Scale -> Rotate -> Translate ** */
	temp = matrix4_multiply(rotation_x_matrix, scale_matrix);
	temp = matrix4_multiply(rotation_y_matrix, temp);
	temp = matrix4_multiply(rotation_z_matrix, temp);
	transform->matrix = matrix4_multiply(translation_matrix, temp);
}

/*
** Add translation to transform
*/
void	transform_translate(t_transform *transform, t_vec3 translation)
{
	transform->translation = vec3_add(transform->translation, translation);
	transform_update_matrix(transform);
}

/*
** Add rotation to transform
*/
void	transform_rotate(t_transform *transform, t_vec3 rotation)
{
	transform->rotation = vec3_add(transform->rotation, rotation);
	transform_update_matrix(transform);
}

/*
** Apply uniform scale to transform
*/
void	transform_scale_uniform(t_transform *transform, double scale)
{
	transform->scale = vec3_mult(transform->scale, scale);
	transform_update_matrix(transform);
}

/*
** Apply non-uniform scale to transform
*/
void	transform_scale(t_transform *transform, t_vec3 scale)
{
	transform->scale.x *= scale.x;
	transform->scale.y *= scale.y;
	transform->scale.z *= scale.z;
	transform_update_matrix(transform);
}

/*
** Transform a sphere
*/
void	transform_sphere(t_sphere *sphere, t_transform *transform)
{
	sphere->center = matrix4_transform_point(transform->matrix, sphere->center);
	/* ** For sphere diameter, use uniform scale factor ** */
	if (transform->scale.x == transform->scale.y && transform->scale.y == transform->scale.z)
		sphere->diameter *= transform->scale.x;
}

/*
** Transform a plane
*/
void	transform_plane(t_plane *plane, t_transform *transform)
{
	plane->point = matrix4_transform_point(transform->matrix, plane->point);
	plane->normal = matrix4_transform_direction(transform->matrix, plane->normal);
}

/*
** Transform a cylinder
*/
void	transform_cylinder(t_cylinder *cylinder, t_transform *transform)
{
	cylinder->center = matrix4_transform_point(transform->matrix, cylinder->center);
	cylinder->axis = matrix4_transform_direction(transform->matrix, cylinder->axis);
	/* ** For cylinder diameter, use uniform scale factor ** */
	if (transform->scale.x == transform->scale.y && transform->scale.y == transform->scale.z)
	{
		cylinder->diameter *= transform->scale.x;
		cylinder->height *= transform->scale.y;
	}
}

/*
** Transform a cone
*/
void	transform_cone(t_cone *cone, t_transform *transform)
{
	cone->vertex = matrix4_transform_point(transform->matrix, cone->vertex);
	cone->axis = matrix4_transform_direction(transform->matrix, cone->axis);
	/* ** For cone height, use uniform scale factor ** */
	if (transform->scale.x == transform->scale.y && transform->scale.y == transform->scale.z)
		cone->height *= transform->scale.y;
}

/*
** Transform a camera
*/
void	transform_camera(t_camera *camera, t_transform *transform)
{
	camera->position = matrix4_transform_point(transform->matrix, camera->position);
	camera->orientation = matrix4_transform_direction(transform->matrix, camera->orientation);
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
		transform_cylinder(&scene->objects[obj_index].data.cylinder, &transform);
	else if (scene->objects[obj_index].type == CONE)
		transform_cone(&scene->objects[obj_index].data.cone, &transform);
}

/*
** Rotate object in scene
*/
void	scene_rotate_object(t_scene *scene, int obj_index, t_vec3 rotation)
{
	t_transform	transform;

	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;
	transform = transform_identity();
	transform_rotate(&transform, rotation);
	if (scene->objects[obj_index].type == PLANE)
		transform_plane(&scene->objects[obj_index].data.plane, &transform);
	else if (scene->objects[obj_index].type == CYLINDER)
		transform_cylinder(&scene->objects[obj_index].data.cylinder, &transform);
	else if (scene->objects[obj_index].type == CONE)
		transform_cone(&scene->objects[obj_index].data.cone, &transform);
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
		transform_cylinder(&scene->objects[obj_index].data.cylinder, &transform);
	else if (scene->objects[obj_index].type == CONE)
		transform_cone(&scene->objects[obj_index].data.cone, &transform);
}

/*
** Translate camera in scene
*/
void	scene_translate_camera(t_scene *scene, t_vec3 delta)
{
	t_transform	transform;

	transform = transform_identity();
	transform_translate(&transform, delta);
	transform_camera(&scene->camera, &transform);
}

/*
** Rotate camera in scene
*/
void	scene_rotate_camera(t_scene *scene, t_vec3 rotation)
{
	t_transform	transform;

	transform = transform_identity();
	transform_rotate(&transform, rotation);
	transform_camera(&scene->camera, &transform);
}
