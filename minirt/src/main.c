#include "../includes/minirt.h"
#include "../includes/main.h"
#include "../includes/events.h"
#include "../includes/draw.h"
#include <stdio.h>

/**
 * Print error message and exit
 */
void	error_exit(char *message)
{
	ft_fprintf_fd(2, "%s", message);
	exit(EXIT_FAILURE);
}

static void	print_object_info(t_object *obj, int index)
{
	t_sphere	*sp;
	t_plane		*pl;
	t_cylinder	*cy;

	if (obj->type == SPHERE)
	{
		sp = &obj->data.sphere;
		printf("  Sphere %d: center=(%.2f,%.2f,%.2f), diam=%.2f, color=(%.2f,%.2f,%.2f)\n",
			index + 1, sp->center.x, sp->center.y, sp->center.z,
			sp->diameter, sp->color.x, sp->color.y, sp->color.z);
	}
	else if (obj->type == PLANE)
	{
		pl = &obj->data.plane;
		printf("  Plane %d: point=(%.2f,%.2f,%.2f), normal=(%.2f,%.2f,%.2f), color=(%.2f,%.2f,%.2f)\n",
			index + 1, pl->point.x, pl->point.y, pl->point.z,
			pl->normal.x, pl->normal.y, pl->normal.z,
			pl->color.x, pl->color.y, pl->color.z);
	}
	else if (obj->type == CYLINDER)
	{
		cy = &obj->data.cylinder;
		printf("  Cylinder %d: center=(%.2f,%.2f,%.2f), axis=(%.2f,%.2f,%.2f), diam=%.2f, height=%.2f, color=(%.2f,%.2f,%.2f)\n",
			index + 1, cy->center.x, cy->center.y, cy->center.z,
			cy->axis.x, cy->axis.y, cy->axis.z,
			cy->diameter, cy->height, cy->color.x, cy->color.y, cy->color.z);
	}
}

/**
 * Print scene information for debugging
 */
void	print_scene_info(t_scene *scene)
{
	int	i;

	printf("Scene Information:\n");
	printf("Camera: pos=(%.2f,%.2f,%.2f), dir=(%.2f,%.2f,%.2f), fov=%.2f\n",
		scene->camera.position.x, scene->camera.position.y, scene->camera.position.z,
		scene->camera.orientation.x, scene->camera.orientation.y, scene->camera.orientation.z,
		scene->camera.fov);
	printf("Objects (%d):\n", scene->num_objects);
	i = 0;
	while (i < scene->num_objects)
	{
		print_object_info(&scene->objects[i], i);
		i++;
	}
}

static void	init_mlx_and_window(t_vars *vars)
{
	vars->mlx = mlx_init();
	if (!vars->mlx)
		error_exit("Error: MLX initialization failed\n");
	vars->win = mlx_new_window(vars->mlx, WIDTH, HEIGHT, "miniRT");
	if (!vars->win)
		error_exit("Error: Window creation failed\n");
}

/**
 * Main function
 */
int	main(int argc, char **argv)
{
	t_scene	*scene;
	t_vars	vars;

	if (argc != 2)
		error_exit(ERR_ARGS);
	scene = parse_scene_file(argv[1]);
	if (!scene)
		error_exit(ERR_SCENE);
	print_scene_info(scene);
	init_mlx_and_window(&vars);
	create_image(&vars);
	main_draw(&vars, scene);
	mlx_hooks(&vars);
	mlx_put_image_to_window(vars.mlx, vars.win, vars.img->img, 0, 0);
	mlx_loop(vars.mlx);
	free(scene);
	return (0);
}
