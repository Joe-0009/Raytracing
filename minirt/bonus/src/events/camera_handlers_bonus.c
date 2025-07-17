#include "../../includes/events_bonus.h"
#include "../../includes/minirt_app_bonus.h"
#include "../../includes/scene_bonus.h"
#include <stdio.h>

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
		rotation.x = 0.1;
	else if (keycode == KEY_K)
		rotation.x = -0.1;
	else if (keycode == KEY_J)
		rotation.y = -0.1;
	else if (keycode == KEY_L)
		rotation.y = 0.1;
	else
		return;
	
	scene_rotate_camera(scene, rotation);
}
