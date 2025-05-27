#include "../../includes/draw.h"
#include "../../includes/main.h"
#include "../../includes/raytrace.h"
#include "../../includes/scene.h"
#include <stdio.h>

static void	cleanup_on_failure(t_vars *vars)
{
	if (vars->img)
	{
		if (vars->img->img)
			mlx_destroy_image(vars->mlx, vars->img->img);
		free(vars->img);
		vars->img = NULL;
	}
	exit(EXIT_FAILURE);
}

void	create_image(t_vars *vars)
{
	vars->img = malloc(sizeof(t_image));
	if (!vars->img)
		cleanup_on_failure(vars);
	vars->img->img = mlx_new_image(vars->mlx, WIDTH, HEIGHT);
	if (!vars->img->img)
		cleanup_on_failure(vars);
	vars->img->addr = mlx_get_data_addr(vars->img->img,
			&vars->img->bits_per_pixel, &vars->img->line_length,
			&vars->img->endian);
	if (!vars->img->addr)
		cleanup_on_failure(vars);
}

void	put_pixel(t_vars *vars, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		dst = vars->img->addr + (y * vars->img->line_length + x
				* (vars->img->bits_per_pixel / 8));
		*(unsigned int *)dst = color;
	}
}

void	main_draw(t_vars *vars, t_scene *scene)
{
	t_ray	ray;
	int		x;
	int		y;
	int		color;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			ray = generate_camera_ray(scene, x, y);
			color = trace_ray(scene, ray);
			put_pixel(vars, x, y, color);
			x++;
		}
		y++;
	}
}
