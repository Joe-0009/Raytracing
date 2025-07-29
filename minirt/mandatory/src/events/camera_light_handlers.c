#include "../../includes/events.h"
#include "../../includes/minirt_app.h"
#include "../../includes/scene.h"
#include <stdio.h>

void	handle_camera_rotation(int keycode, t_scene *scene)
{
	t_vec3	rotation;

	rotation = vec3_create(0, 0, 0);
	if (keycode == KEY_I)
		rotation.x = 0.1;
	else if (keycode == KEY_K)
		rotation.x = -0.1;
	else if (keycode == KEY_J)
		rotation.y = 0.1;
	else if (keycode == KEY_L)
		rotation.y = -0.1;
	else
		return ;
	scene->camera.orientation = vec3_rotate(scene->camera.orientation,
			rotation);
}

void	handle_camera_movement(int keycode, t_scene *scene)
{
	t_vec3	movement;

	movement = vec3_create(0, 0, 0);
	if (keycode == KEY_W)
		movement = vec3_mult(scene->camera.orientation, 0.5);
	else if (keycode == KEY_S)
		movement = vec3_mult(scene->camera.orientation, -0.5);
	else if (keycode == KEY_A)
		movement = vec3_create(-0.5, 0, 0);
	else if (keycode == KEY_D)
		movement = vec3_create(0.5, 0, 0);
	else if (keycode == KEY_Q)
		movement = vec3_create(0, -0.5, 0);
	else if (keycode == KEY_E)
		movement = vec3_create(0, 0.5, 0);
	else
		return ;
	scene->camera.position = vec3_add(scene->camera.position, movement);
}

void	handle_light_movement(int keycode, t_scene *scene)
{
	t_vec3	movement;

	movement = vec3_create(0, 0, 0);
	if (keycode == KEY_V)
		movement = vec3_create(-0.5, 0, 0);
	else if (keycode == KEY_B)
		movement = vec3_create(0.5, 0, 0);
	else if (keycode == KEY_N)
		movement = vec3_create(0, -0.5, 0);
	else if (keycode == KEY_M)
		movement = vec3_create(0, 0.5, 0);
	else if (keycode == KEY_COMMA)
		movement = vec3_create(0, 0, -0.5);
	else if (keycode == KEY_DOT)
		movement = vec3_create(0, 0, 0.5);
	else
		return ;
	scene->light.position = vec3_add(scene->light.position, movement);
}

