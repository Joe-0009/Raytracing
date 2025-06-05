#include "../../includes/minirt_app.h"
#include "../../includes/scene_math.h"
#include <stdio.h>

/*
** Create a new image for rendering
*/
void	create_image(t_vars *vars)
{
	vars->img = malloc(sizeof(t_image));
	if (!vars->img)
		exit(EXIT_FAILURE);
	vars->img->img = mlx_new_image(vars->mlx, WIDTH, HEIGHT);
	if (!vars->img->img)
		exit(EXIT_FAILURE);
	vars->img->addr = mlx_get_data_addr(vars->img->img,
		&vars->img->bits_per_pixel, &vars->img->line_length,
		&vars->img->endian);
	if (!vars->img->addr)
		exit(EXIT_FAILURE);
}

/*
** Put a pixel of a given color at (x, y)
*/
void	put_pixel(t_vars *vars, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		dst = vars->img->addr + (y * vars->img->line_length
			+ x * (vars->img->bits_per_pixel / 8));
		*(unsigned int *)dst = color;
	}
}

/*
** Main draw loop for the scene
*/
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
