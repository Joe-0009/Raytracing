#include "../../includes/events_bonus.h"
#include "../../includes/minirt_app_bonus.h"
#include "../../includes/scene_bonus.h"
#include <stdio.h>

/*
** Translate camera directly using matrix operations
*/
void	scene_translate_camera(t_scene *scene, t_vec3 movement)
{
	t_matrix4	translation_matrix;

	translation_matrix = matrix4_translation(movement);
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

void	handle_camera_movement(int keycode, t_scene *scene)
{
	t_vec3	movement;
	t_vec3	right;
	t_vec3	world_up;

	movement = vec3_create(0, 0, 0);
	world_up = vec3_create(0, 1, 0);
	
	if (keycode == KEY_W)
		movement = vec3_mult(scene->camera.orientation, 0.5);
	else if (keycode == KEY_S)
		movement = vec3_mult(scene->camera.orientation, -0.5);
	else if (keycode == KEY_A)
	{
		right = vec3_cross(scene->camera.orientation, world_up);
		movement = vec3_mult(vec3_normalize(right), -0.5);
	}
	else if (keycode == KEY_D)
	{
		right = vec3_cross(scene->camera.orientation, world_up);
		movement = vec3_mult(vec3_normalize(right), 0.5);
	}
	else if (keycode == KEY_Q)
		movement = vec3_create(0, -0.5, 0);
	else if (keycode == KEY_E)
		movement = vec3_create(0, 0.5, 0);
	else
		return;
	
	scene_translate_camera(scene, movement);
}

void	handle_camera_rotation(int keycode, t_scene *scene)
{
	t_vec3	rotation;

	rotation = vec3_create(0, 0, 0);
	
	if (keycode == KEY_I)
		rotation.x = 0.1;		// Pitch up
	else if (keycode == KEY_K)
		rotation.x = -0.1;		// Pitch down
	else if (keycode == KEY_J)
		rotation.y = -0.1;		// Yaw left
	else if (keycode == KEY_L)
		rotation.y = 0.1;		// Yaw right
	else
		return;
	
	scene_rotate_camera(scene, rotation);
}
