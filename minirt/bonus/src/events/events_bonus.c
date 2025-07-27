#include "../../includes/events_bonus.h"
#include "../../includes/minirt_app_bonus.h"
#include "../../includes/scene_bonus.h"
#include <stdio.h>

void	draw_new_image(t_vars *vars, t_scene *scene)
{
	mlx_destroy_image(vars->mlx, vars->img->img);
	create_image(vars);
	main_draw(vars, scene);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img->img, 0, 0);
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
		|| keycode == KEY_O || keycode == KEY_U || keycode == KEY_Y
		|| keycode == KEY_H || keycode == KEY_N)
		return (1);
	return (0);
}

int	key_handler(int keycode, t_vars *vars)
{
	if (keycode == 65307)
		close_window_esc(keycode, vars);
	else if (vars->scene)
	{
		handle_camera_movement(keycode, vars->scene);
		handle_camera_rotation(keycode, vars->scene);
		handle_object_transforms(keycode, vars->scene);
		if (is_redraw_key(keycode))
			draw_new_image(vars, vars->scene);
	}
	if (keycode == 32)
		print_controls_help();
	return (0);
}

void	mlx_hooks(t_vars *vars)
{
	mlx_hook(vars->win, 2, 1L << 0, key_handler, vars);
	mlx_hook(vars->win, 17, 0, close_window_x, vars);
}
