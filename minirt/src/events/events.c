#include "../../includes/main.h"
#include "../../includes/events.h"
#include "../../includes/draw.h"
#include "../../includes/transforms.h"
#include <stdio.h>

static int		g_selected_obj = 0;

/* Function declarations */
void	print_controls_help(void);

void	draw_new_image(t_vars *vars, t_scene *scene)
{
	create_image(vars);
	main_draw(vars, scene);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img->img, 0, 0);
}

int	close_window_x(t_vars *vars)
{
	(void)vars;
	exit(0);
	return (0);
}

int	close_window_esc(int keycode, t_vars *vars)
{
	(void)vars;
	if (keycode == KEY_ESC || keycode == KEY_ESC_MAC)
		exit(0);
	return (0);
}

static int	is_redraw_key_mac(int keycode)
{
	if (keycode == 13 || keycode == 0 || keycode == 1 || keycode == 2 ||
		keycode == 12 || keycode == 14 || keycode == 34 || keycode == 38 ||
		keycode == 40 || keycode == 37 || keycode == 126 || keycode == 125 ||
		keycode == 123 || keycode == 124 || keycode == 24 || keycode == 27 ||
		keycode == 15 || keycode == 17 || keycode == 3 || keycode == 5 ||
		keycode == 35 || keycode == 31)
		return (1);
	return (0);
}

static int	is_redraw_key(int keycode)
{
	if (keycode == KEY_W || keycode == KEY_A || keycode == KEY_S ||
		keycode == KEY_D || keycode == KEY_Q || keycode == KEY_E ||
		keycode == KEY_I || keycode == KEY_J || keycode == KEY_K ||
		keycode == KEY_L || keycode == KEY_UP || keycode == KEY_DOWN ||
		keycode == KEY_LEFT || keycode == KEY_RIGHT || keycode == KEY_PLUS ||
		keycode == KEY_MINUS || keycode == KEY_R || keycode == KEY_T ||
		keycode == KEY_F || keycode == KEY_G || keycode == KEY_P ||
		keycode == KEY_O)
		return (1);
	return (is_redraw_key_mac(keycode));
}

int	key_handler(int keycode, t_vars *vars)
{
	if (keycode == KEY_ESC || keycode == KEY_ESC_MAC)  /* ESC key */
		close_window_esc(keycode, vars);
	else if (g_scene)
	{
		handle_camera_movement(keycode, g_scene);
		handle_camera_rotation(keycode, g_scene);
		handle_object_transforms(keycode, g_scene);
		/* ** Redraw scene after transformation ** */
		if (is_redraw_key(keycode))
		{
			draw_new_image(vars, g_scene);
		}
	}
	if (keycode == KEY_SPACE || keycode == KEY_SPACE_MAC)
		print_controls_help();
	return (0);
}

void	mlx_hooks(t_vars *vars)
{
	mlx_hook(vars->win, 2, 1L << 0, key_handler, vars);
	mlx_hook(vars->win, 17, 0, close_window_x, vars);
}

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

/*
** Handle object transformation controls
*/
void	handle_object_transforms(int keycode, t_scene *scene)
{
	/* ** Object selection with P/O keys ** */
	if ((keycode == KEY_P || keycode == KEY_P_MAC) && g_selected_obj < scene->num_objects - 1)
		g_selected_obj++;
	else if ((keycode == KEY_O || keycode == KEY_O_MAC) && g_selected_obj > 0)
		g_selected_obj--;
	/* ** Object translation with arrow keys ** */
	else if (keycode == KEY_LEFT || keycode == KEY_LEFT_MAC)
		scene_translate_object(scene, g_selected_obj, vec3_create(-0.3, 0, 0));
	else if (keycode == KEY_RIGHT || keycode == KEY_RIGHT_MAC)
		scene_translate_object(scene, g_selected_obj, vec3_create(0.3, 0, 0));
	else if (keycode == KEY_UP || keycode == KEY_UP_MAC)
		scene_translate_object(scene, g_selected_obj, vec3_create(0, 0.3, 0));
	else if (keycode == KEY_DOWN || keycode == KEY_DOWN_MAC)
		scene_translate_object(scene, g_selected_obj, vec3_create(0, -0.3, 0));
	/* ** Object scaling with +/- keys ** */
	else if (keycode == KEY_PLUS || keycode == KEY_PLUS_MAC)
		scene_scale_object(scene, g_selected_obj, 1.1);
	else if (keycode == KEY_MINUS || keycode == KEY_MINUS_MAC)
		scene_scale_object(scene, g_selected_obj, 0.9);
	/* ** Object rotation with R/T keys (forward) and F/G keys (reverse) ** */
	else if (keycode == KEY_R || keycode == KEY_R_MAC)
		scene_rotate_object(scene, g_selected_obj, vec3_create(0.2, 0, 0));
	else if (keycode == KEY_T || keycode == KEY_T_MAC)
		scene_rotate_object(scene, g_selected_obj, vec3_create(0, 0.2, 0));
	else if (keycode == KEY_F || keycode == KEY_F_MAC)
		scene_rotate_object(scene, g_selected_obj, vec3_create(-0.2, 0, 0));
	else if (keycode == KEY_G || keycode == KEY_G_MAC)
		scene_rotate_object(scene, g_selected_obj, vec3_create(0, -0.2, 0));
}




