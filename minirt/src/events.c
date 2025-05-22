#include "../includes/minirt.h"
#include "../main.h"

// void	draw_new_image(t_vars *vars)
// {
// 	cleanup_image(vars);
// 	create_image(vars);
// 	main_draw(vars);
// 	mlx_put_image_to_window(vars->mlx, vars->win, vars->img->img, 0, 0);
// }

int	close_window_x(t_vars *vars)
{
	cleanup_all(vars);
	exit(0);
	return (0);
}

int	close_window_esc(int keycode, t_vars *vars)
{
	if (keycode == KEY_ESC)
	{
		cleanup_all(vars);
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

// Need to implement this function to satisfy the function call in events.c
void	cleanup_all(t_vars *vars)
{
	// Clean up image if it exists
	if (vars->img)
	{
		if (vars->img->img)
			mlx_destroy_image(vars->mlx, vars->img->img);
		free(vars->img);
	}
	
	// Clean up window if it exists
	if (vars->win)
		mlx_destroy_window(vars->mlx, vars->win);
}
