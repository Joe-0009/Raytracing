#include "../includes/events_bonus.h"
#include "../includes/scene_bonus.h"
#include <stdio.h>

/*
** Translate camera directly using matrix operations
*/
void	scene_translate_camera(t_scene *scene, t_vec3 delta)
{
	t_matrix4	translation_matrix;

	translation_matrix = matrix4_translation(delta);
	scene->camera.position = matrix4_transform_point(translation_matrix,
			scene->camera.position);
}

/*
** Rotate camera using direct matrix operations
** Applies rotation in world space (XYZ order)
*/
void	scene_rotate_camera(t_scene *scene, t_vec3 rotation)
{
	t_matrix4	rotation_x;
	t_matrix4	rotation_y;
	t_matrix4	rotation_z;
	t_matrix4	combined_rotation;

	// Create individual rotation matrices
	rotation_x = matrix4_rotation_x(rotation.x);
	rotation_y = matrix4_rotation_y(rotation.y);
	rotation_z = matrix4_rotation_z(rotation.z);

	// Combine rotations: Z * Y * X (standard order)
	combined_rotation = matrix4_multiply(rotation_z, rotation_y);
	combined_rotation = matrix4_multiply(combined_rotation, rotation_x);

	// Apply rotation to camera orientation
	scene->camera.orientation = matrix4_transform_direction(combined_rotation,
			scene->camera.orientation);
	scene->camera.orientation = vec3_normalize(scene->camera.orientation);
}

/*
** Rotate camera in local space (relative to camera's current orientation)
** This provides more intuitive FPS-style camera controls
*/
// void	scene_rotate_camera_local(t_scene *scene, t_vec3 rotation)
// {
// 	t_vec3		world_up;
// 	t_vec3		right;
// 	t_vec3		up;
// 	t_matrix4	pitch_matrix;
// 	t_matrix4	yaw_matrix;
// 	t_matrix4	roll_matrix;
// 	t_matrix4	combined_rotation;

// 	world_up = vec3_create(0, 1, 0);
	
// 	// Calculate camera's local coordinate system
// 	right = vec3_normalize(vec3_cross(scene->camera.orientation, world_up));
// 	up = vec3_cross(right, scene->camera.orientation);

// 	// Create rotation matrices around local axes
// 	pitch_matrix = matrix4_rotation_x(right);		// Pitch around right axis
// 	yaw_matrix = matrix4_rotation_y(world_up);		// Yaw around world up
// 	roll_matrix = matrix4_rotation_z(scene->camera.orientation);	// Roll around forward

// 	// Combine rotations: Yaw * Pitch * Roll
// 	combined_rotation = matrix4_multiply(yaw_matrix, pitch_matrix);
// 	combined_rotation = matrix4_multiply(combined_rotation, roll_matrix);

// 	// Apply to camera orientation
// 	scene->camera.orientation = matrix4_transform_direction(combined_rotation,
// 			scene->camera.orientation);
// 	scene->camera.orientation = vec3_normalize(scene->camera.orientation);
// }

/*
** Combined camera transformation (translation + rotation)
*/
void	scene_transform_camera(t_scene *scene, t_vec3 translation, t_vec3 rotation)
{
	// Apply translation first
	scene_translate_camera(scene, translation);
	
	// Then apply rotation
	scene_rotate_camera(scene, rotation);
}

/*
** Rotate camera around a specific axis by angle
*/
void	scene_rotate_camera_axis(t_scene *scene, t_vec3 axis, double angle)
{
	t_matrix4	rotation_matrix;

	rotation_matrix = matrix4_rotation_axis(vec3_normalize(axis), angle);
	scene->camera.orientation = matrix4_transform_direction(rotation_matrix,
			scene->camera.orientation);
	scene->camera.orientation = vec3_normalize(scene->camera.orientation);
}

/*
** Rotate camera around X axis only
*/
void	scene_rotate_camera_x(t_scene *scene, double angle)
{
	t_matrix4	rotation_matrix;

	rotation_matrix = matrix4_rotation_x(angle);
	scene->camera.orientation = matrix4_transform_direction(rotation_matrix,
			scene->camera.orientation);
	scene->camera.orientation = vec3_normalize(scene->camera.orientation);
}

/*
** Rotate camera around Y axis only
*/
void	scene_rotate_camera_y(t_scene *scene, double angle)
{
	t_matrix4	rotation_matrix;

	rotation_matrix = matrix4_rotation_y(angle);
	scene->camera.orientation = matrix4_transform_direction(rotation_matrix,
			scene->camera.orientation);
	scene->camera.orientation = vec3_normalize(scene->camera.orientation);
}

/*
** Rotate camera around Z axis only
*/
void	scene_rotate_camera_z(t_scene *scene, double angle)
{
	t_matrix4	rotation_matrix;

	rotation_matrix = matrix4_rotation_z(angle);
	scene->camera.orientation = matrix4_transform_direction(rotation_matrix,
			scene->camera.orientation);
	scene->camera.orientation = vec3_normalize(scene->camera.orientation);
}
