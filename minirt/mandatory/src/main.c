#include "../includes/events.h"
#include "../includes/minirt_app.h"
#include "../includes/render_utils.h"
#include <stdio.h>

t_scene	*g_scene = NULL;

void	error_exit(char *message)
{
	printf("%s", message);
	exit(EXIT_FAILURE);
}

static void	print_object_info(t_object *obj, int index)
{
	if (obj->type == SPHERE)
		printf("  Sphere %d: center=(%.2f,%.2f,%.2f), diam=%.2f, "
			"color=(%.2f,%.2f,%.2f)\n", index + 1,
			obj->data.sphere.center.x, obj->data.sphere.center.y,
			obj->data.sphere.center.z, obj->data.sphere.diameter,
			obj->data.sphere.color.x, obj->data.sphere.color.y,
			obj->data.sphere.color.z);
	else if (obj->type == PLANE)
		printf("  Plane %d: point=(%.2f,%.2f,%.2f), "
			"normal=(%.2f,%.2f,%.2f), color=(%.2f,%.2f,%.2f)\n",
			index + 1, obj->data.plane.point.x, obj->data.plane.point.y,
			obj->data.plane.point.z, obj->data.plane.normal.x,
			obj->data.plane.normal.y, obj->data.plane.normal.z,
			obj->data.plane.color.x, obj->data.plane.color.y,
			obj->data.plane.color.z);
	else if (obj->type == CYLINDER)
		printf("  Cylinder %d: center=(%.2f,%.2f,%.2f), "
			"axis=(%.2f,%.2f,%.2f), diam=%.2f, height=%.2f, "
			"color=(%.2f,%.2f,%.2f)\n", index + 1,
			obj->data.cylinder.center.x, obj->data.cylinder.center.y,
			obj->data.cylinder.center.z, obj->data.cylinder.axis.x,
			obj->data.cylinder.axis.y, obj->data.cylinder.axis.z,
			obj->data.cylinder.diameter, obj->data.cylinder.height,
			obj->data.cylinder.color.x, obj->data.cylinder.color.y,
			obj->data.cylinder.color.z);
	else if (obj->type == CONE)
		printf("  Cone %d: vertex=(%.2f,%.2f,%.2f), "
			"axis=(%.2f,%.2f,%.2f), angle=%.2f, height=%.2f, "
			"color=(%.2f,%.2f,%.2f)\n", index + 1,
			obj->data.cone.vertex.x, obj->data.cone.vertex.y,
			obj->data.cone.vertex.z, obj->data.cone.axis.x,
			obj->data.cone.axis.y, obj->data.cone.axis.z,
			obj->data.cone.angle * 180.0 / M_PI, obj->data.cone.height,
			obj->data.cone.color.x, obj->data.cone.color.y,
			obj->data.cone.color.z);
}

static void	print_scene_basic_info(t_scene *scene)
{
	printf("Scene Information:\n");
	printf("Ambient: ratio=%.2f, color=(%.0f,%.0f,%.0f)\n",
		scene->ambient.ratio, scene->ambient.color.x * 255,
		scene->ambient.color.y * 255, scene->ambient.color.z * 255);
	printf("Light: pos=(%.2f,%.2f,%.2f), brightness=%.2f, "
		"color=(%.0f,%.0f,%.0f)\n",
		scene->light.position.x, scene->light.position.y,
		scene->light.position.z, scene->light.brightness,
		scene->light.color.x * 255, scene->light.color.y * 255,
		scene->light.color.z * 255);
	printf("Camera: pos=(%.2f,%.2f,%.2f), dir=(%.2f,%.2f,%.2f), "
		"fov=%.2f\n", scene->camera.position.x, scene->camera.position.y,
		scene->camera.position.z, scene->camera.orientation.x,
		scene->camera.orientation.y, scene->camera.orientation.z,
		scene->camera.fov);
}

void	print_scene_info(t_scene *scene)
{
	int	i;

	print_scene_basic_info(scene);
	printf("Objects (%d):\n", scene->num_objects);
	i = 0;
	while (i < scene->num_objects)
	{
		print_object_info(&scene->objects[i], i);
		i++;
	}
}

void	set_scene_for_transforms(t_scene *scene)
{
	g_scene = scene;
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
	print_scene_info(scene);
	init_mlx_and_window(&vars);
	set_scene_for_transforms(scene);
	main_draw(&vars, scene);
	mlx_hooks(&vars);
	mlx_put_image_to_window(vars.mlx, vars.win, vars.img->img, 0, 0);
	mlx_loop(vars.mlx);
	free(scene);
	return (0);
}
