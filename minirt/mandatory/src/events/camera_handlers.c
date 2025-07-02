#include "../../includes/events.h"
#include "../../includes/minirt_app.h"
#include "../../includes/scene.h"
#include <stdio.h>

void	handle_camera_movement(int keycode, t_scene *scene)
{
	t_vec3	right;

	if (keycode == KEY_W)
		scene_translate_camera(scene, vec3_mult(scene->camera.orientation,
				0.5));
	else if (keycode == KEY_S)
		scene_translate_camera(scene, vec3_mult(scene->camera.orientation,
				-0.5));
	else if (keycode == KEY_A)
	{
		right = vec3_cross(scene->camera.orientation, vec3_create(0, 1, 0));
		scene_translate_camera(scene, vec3_mult(vec3_normalize(right), -0.5));
	}
	else if (keycode == KEY_D)
	{
		right = vec3_cross(scene->camera.orientation, vec3_create(0, 1, 0));
		scene_translate_camera(scene, vec3_mult(vec3_normalize(right), 0.5));
	}
	else if (keycode == KEY_Q)
		scene_translate_camera(scene, vec3_create(0, -0.5, 0));
	else if (keycode == KEY_E)
		scene_translate_camera(scene, vec3_create(0, 0.5, 0));
}

void	handle_camera_rotation(int keycode, t_scene *scene)
{
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;
	t_vec3	world_up;
	double	angle;

	if (!(keycode == KEY_I || keycode == KEY_K
			|| keycode == KEY_J || keycode == KEY_L))
		return ;
	forward = vec3_normalize(scene->camera.orientation);
	world_up = vec3_create(0, 1, 0);
	right = vec3_normalize(vec3_cross(forward, world_up));
	up = vec3_cross(right, forward);
	angle = 0.1;
	if (keycode == KEY_I)
		scene->camera.orientation = vec3_add(vec3_mult(forward, cos(angle)),
				vec3_mult(up, sin(angle)));
	else if (keycode == KEY_K)
		scene->camera.orientation = vec3_add(vec3_mult(forward, cos(-angle)),
				vec3_mult(up, sin(-angle)));
	else if (keycode == KEY_J)
		scene->camera.orientation = vec3_add(vec3_mult(forward, cos(-angle)),
				vec3_mult(right, sin(-angle)));
	else if (keycode == KEY_L)
		scene->camera.orientation = vec3_add(vec3_mult(forward, cos(angle)),
				vec3_mult(right, sin(angle)));
	scene->camera.orientation = vec3_normalize(scene->camera.orientation);
}
