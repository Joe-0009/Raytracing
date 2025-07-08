#include "../includes/events_bonus.h"
#include "../includes/minirt_app_bonus.h"
#include "../includes/parser_bonus.h"
#include "../includes/render_utils_bonus.h"
#include "../includes/scene_bonus.h"
#include <stdio.h>

# include <mlx.h>

void	error_exit(char *message)
{
	printf("%s", message);
	exit(EXIT_FAILURE);
}

static void	init_mlx_and_window(t_vars *vars)
{
	vars->mlx = mlx_init();
	if (!vars->mlx)
		error_exit("Error: MLX initialization failed\n");
	vars->win = mlx_new_window(vars->mlx, WIDTH, HEIGHT, "miniRT");
	if (!vars->win)
		error_exit("Error: Window creation failed\n");
	create_image(vars);
}

int	main(int argc, char **argv)
{
	t_scene	*scene;
	t_vars	vars;

	if (argc != 2)
		error_exit(ERR_ARGS);
	scene = parse_scene_file(argv[1]);
	if (!scene)
		error_exit(ERR_SCENE);
	init_mlx_and_window(&vars);
	vars.scene = scene;
	load_scene_texture_bump(vars.mlx, scene);
	main_draw(&vars, scene);
	mlx_hooks(&vars);
	mlx_put_image_to_window(vars.mlx, vars.win, vars.img->img, 0, 0);
	mlx_loop(vars.mlx);
	ft_free_scene(&scene);
	return (0);
}
