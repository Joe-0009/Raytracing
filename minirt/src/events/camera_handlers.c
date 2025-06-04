#include "../../includes/main.h"
#include "../../includes/events.h"
#include "../../includes/transforms.h"
#include <stdio.h>

/*
** Handle camera movement controls
*/
void	handle_camera_movement(int keycode, t_scene *scene)
{
	if (keycode == KEY_W || keycode == KEY_W_MAC)
		scene_translate_camera(scene, vec3_mult(scene->camera.orientation, 0.5));
	else if (keycode == KEY_S || keycode == KEY_S_MAC)
		scene_translate_camera(scene, vec3_mult(scene->camera.orientation, -0.5));
	else if (keycode == KEY_A || keycode == KEY_A_MAC)
	{
		t_vec3 right = vec3_cross(scene->camera.orientation, vec3_create(0, 1, 0));
		scene_translate_camera(scene, vec3_mult(vec3_normalize(right), -0.5));
	}
	else if (keycode == KEY_D || keycode == KEY_D_MAC)
	{
		t_vec3 right = vec3_cross(scene->camera.orientation, vec3_create(0, 1, 0));
		scene_translate_camera(scene, vec3_mult(vec3_normalize(right), 0.5));
	}
	else if (keycode == KEY_Q || keycode == KEY_Q_MAC)
		scene_translate_camera(scene, vec3_create(0, -0.5, 0));
	else if (keycode == KEY_E || keycode == KEY_E_MAC)
		scene_translate_camera(scene, vec3_create(0, 0.5, 0));
}

/*
** Handle camera rotation controls (first-person style)
*/
void	handle_camera_rotation(int keycode, t_scene *scene)
{
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;
	t_vec3	world_up;
	double	angle;

	if (!(keycode == KEY_I || keycode == KEY_I_MAC || keycode == KEY_K || keycode == KEY_K_MAC ||
		  keycode == KEY_J || keycode == KEY_J_MAC || keycode == KEY_L || keycode == KEY_L_MAC))
		return ;

	/* ** Get camera coordinate system ** */
	forward = vec3_normalize(scene->camera.orientation);
	world_up = vec3_create(0, 1, 0);
	right = vec3_normalize(vec3_cross(forward, world_up));
	up = vec3_cross(right, forward);
	angle = 0.1;

	/* ** Apply rotation around local axes ** */
	if (keycode == KEY_I || keycode == KEY_I_MAC)  /* Look up - rotate around right axis */
	{
		scene->camera.orientation = vec3_add(
			vec3_mult(forward, cos(angle)),
			vec3_mult(up, sin(angle))
		);
	}
	else if (keycode == KEY_K || keycode == KEY_K_MAC)  /* Look down - rotate around right axis */
	{
		scene->camera.orientation = vec3_add(
			vec3_mult(forward, cos(-angle)),
			vec3_mult(up, sin(-angle))
		);
	}
	else if (keycode == KEY_J || keycode == KEY_J_MAC)  /* Look left - rotate around up axis */
	{
		scene->camera.orientation = vec3_add(
			vec3_mult(forward, cos(-angle)),
			vec3_mult(right, sin(-angle))
		);
	}
	else if (keycode == KEY_L || keycode == KEY_L_MAC)  /* Look right - rotate around up axis */
	{
		scene->camera.orientation = vec3_add(
			vec3_mult(forward, cos(angle)),
			vec3_mult(right, sin(angle))
		);
	}
	
	/* ** Normalize the new orientation ** */
	scene->camera.orientation = vec3_normalize(scene->camera.orientation);
} 
