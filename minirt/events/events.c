#include "../includes/minirt.h"
#include "../includes/main.h"

// void	draw_new_image(t_vars *vars)
// {
// 	cleanup_image(vars);
// 	create_image(vars);
// 	main_draw(vars);
// 	mlx_put_image_to_window(vars->mlx, vars->win, vars->img->img, 0, 0);
// }

int	close_window_x(t_vars *vars)
{
	(void) vars;
	exit(0);
	return (0);
}

int	close_window_esc(int keycode, t_vars *vars)
{
	(void) vars;
	if (keycode == KEY_ESC)
	{
		exit(0);
	}
	return (0);
}

int	key_handler(int keycode, t_vars *vars)
{
	if (keycode == KEY_ESC)
		close_window_esc(keycode, vars);
	return (0);
}

void	mlx_hooks(t_vars *vars)
{
	mlx_hook(vars->win, 2, 1L << 0, key_handler, vars);
	mlx_hook(vars->win, 17, 0, close_window_x, vars);
}
