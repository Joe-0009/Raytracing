#include "../includes/events_bonus.h"
#include "../includes/scene_bonus.h"
#include <stdio.h>

/*
** Apply transformation matrix to camera
*/
void	transform_camera(t_camera *camera, t_transform *transform)
{
	camera->position = matrix4_transform_point(transform->matrix,
			camera->position);
	camera->orientation = matrix4_transform_direction(transform->matrix,
			camera->orientation);
	camera->orientation = vec3_normalize(camera->orientation);
}

/*
** Direct camera translation without creating transform object
*/
void	scene_translate_camera_direct(t_scene *scene, t_vec3 delta)
{
	t_matrix4	translation_matrix;

	translation_matrix = matrix4_translation(delta);
	scene->camera.position = matrix4_transform_point(translation_matrix,
			scene->camera.position);
}

/*
** Translate camera using transform system
*/
void	scene_translate_camera(t_scene *scene, t_vec3 delta)
{
	t_transform	transform;

	transform = transform_identity();
	transform_translate(&transform, delta);
	transform_camera(&scene->camera, &transform);
}

/*
** Rotate camera using transform system
** Rotation is applied relative to world axes
*/
void	scene_rotate_camera(t_scene *scene, t_vec3 rotation)
{
	t_transform	transform;

	transform = transform_identity();
	transform_rotate(&transform, rotation);
	transform_camera(&scene->camera, &transform);
}

/*
** Rotate camera relative to its local coordinate system
** This is more intuitive for FPS-style camera controls
*/
void	scene_rotate_camera_local(t_scene *scene, t_vec3 rotation)
{
	t_vec3		world_up;
	t_vec3		right;
	t_vec3		up;
	t_matrix4	pitch_matrix;
	t_matrix4	yaw_matrix;
	t_matrix4	roll_matrix;
	t_matrix4	combined_rotation;

	world_up = vec3_create(0, 1, 0);
	right = vec3_normalize(vec3_cross(scene->camera.orientation, world_up));
	up = vec3_cross(right, scene->camera.orientation);

	// Create rotation matrices for each axis
	pitch_matrix = matrix4_rotation_x(right, rotation.x);
	yaw_matrix = matrix4_rotation_y(world_up, rotation.y);
	roll_matrix = matrix4_rotation_z(scene->camera.orientation, rotation.z);

	// Combine rotations: Yaw * Pitch * Roll
	combined_rotation = matrix4_multiply(yaw_matrix, pitch_matrix);
	combined_rotation = matrix4_multiply(combined_rotation, roll_matrix);
	scene->camera.orientation = matrix4_transform_direction(combined_rotation,
			scene->camera.orientation);
	scene->camera.orientation = vec3_normalize(scene->camera.orientation);
}

/*
** Combined camera transformation for smooth movement
*/
void	scene_transform_camera(t_scene *scene, t_vec3 translation, t_vec3 rotation)
{
	t_transform	transform;

	transform = transform_identity();
	transform_translate(&transform, translation);
	transform_rotate(&transform, rotation);
	transform_camera(&scene->camera, &transform);
}
