#include "../../includes/minirt_app.h"
#include "../../includes/events.h"
#include "../../includes/scene_math.h"
#include <stdio.h>

int	g_selected_obj = 0;

void	draw_new_image(t_vars *vars, t_scene *scene)
{
	create_image(vars);
	main_draw(vars, scene);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img->img, 0, 0);
}

static int	is_redraw_key_mac(int keycode)
{
	if (keycode == 13 || keycode == 0 || keycode == 1 || keycode == 2
		|| keycode == 12 || keycode == 14 || keycode == 34 || keycode == 38
		|| keycode == 40 || keycode == 37 || keycode == 126 || keycode == 125
		|| keycode == 123 || keycode == 124 || keycode == 24 || keycode == 27
		|| keycode == 15 || keycode == 17 || keycode == 3 || keycode == 5
		|| keycode == 35 || keycode == 31)
		return (1);
	return (0);
}

static int	is_redraw_key(int keycode)
{
	if (keycode == KEY_W || keycode == KEY_A || keycode == KEY_S
		|| keycode == KEY_D || keycode == KEY_Q || keycode == KEY_E
		|| keycode == KEY_I || keycode == KEY_J || keycode == KEY_K
		|| keycode == KEY_L || keycode == KEY_UP || keycode == KEY_DOWN
		|| keycode == KEY_LEFT || keycode == KEY_RIGHT || keycode == KEY_PLUS
		|| keycode == KEY_MINUS || keycode == KEY_R || keycode == KEY_T
		|| keycode == KEY_F || keycode == KEY_G || keycode == KEY_P
		|| keycode == KEY_O)
		return (1);
	return (is_redraw_key_mac(keycode));
}

int	key_handler(int keycode, t_vars *vars)
{
	if (keycode == KEY_ESC || keycode == KEY_ESC_MAC)
		close_window_esc(keycode, vars);
	else if (g_scene)
	{
		handle_camera_movement(keycode, g_scene);
		handle_camera_rotation(keycode, g_scene);
		handle_object_transforms(keycode, g_scene);
		if (is_redraw_key(keycode))
			draw_new_image(vars, g_scene);
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