#include "../../includes/minirt_app_bonus.h"
#include "../../includes/scene_bonus.h"
#include <stdio.h>

void	create_image(t_vars *vars)
{
	vars->img = malloc(sizeof(t_image));
	if (!vars->img)
		error_exit("Error: Image structure allocation failed\n");
	vars->img->img = mlx_new_image(vars->mlx, WIDTH, HEIGHT);
	if (!vars->img->img)
		error_exit("Error: MLX image creation failed\n");
	vars->img->addr = mlx_get_data_addr(vars->img->img,
			&vars->img->bits_per_pixel, &vars->img->line_length,
			&vars->img->endian);
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

void	render_section(t_vars *vars, t_scene *scene, int start_y, int end_y)
{
	t_ray	ray;
	int		x;
	int		y;
	int		color;

	y = start_y;
	while (y < end_y)
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

void	*render_thread(void *arg)
{
	t_thread_data	*data;

	data = (t_thread_data *)arg;
	render_section(data->vars, data->scene, data->start_y, data->end_y);
	return (NULL);
}

void	main_draw(t_vars *vars, t_scene *scene)
{
	pthread_t		threads[NUM_THREADS];
	t_thread_data	thread_data[NUM_THREADS];
	int				i;
	int				rows_per_thread;
	int				remaining_rows;

	rows_per_thread = HEIGHT / NUM_THREADS;
	remaining_rows = HEIGHT % NUM_THREADS;
	i = 0;
	while (i < NUM_THREADS)
	{
		thread_data[i].vars = vars;
		thread_data[i].scene = scene;
		thread_data[i].start_y = i * rows_per_thread;
		thread_data[i].end_y = (i + 1) * rows_per_thread;
		thread_data[i].thread_id = i;
		if (i == NUM_THREADS - 1)
			thread_data[i].end_y += remaining_rows;
		pthread_create(&threads[i], NULL, render_thread, &thread_data[i]);
		i++;
	}
	i = -1;
	while (++i < NUM_THREADS)
		pthread_join(threads[i], NULL);
}
