#include "../includes/minirt.h"
#include "../main.h"

/**
 * Print error message and exit
 */
void error_exit(char *message)
{
    ft_fprintf_fd(2, "%s", message);
    exit(EXIT_FAILURE);
}

/**
 * Print scene information for debugging
 */
void print_scene_info(t_scene *scene)
{
    int i;
    t_sphere *sp;
    t_plane *pl;
    t_cylinder *cy;
    
    printf("Scene Information:\n");
    
    /* Print ambient lighting */
    printf("Ambient: ratio=%.2f, color=(%.2f,%.2f,%.2f)\n", 
           scene->ambient.ratio, 
           scene->ambient.color.x, scene->ambient.color.y, scene->ambient.color.z);
    
    /* Print camera */
    printf("Camera: pos=(%.2f,%.2f,%.2f), dir=(%.2f,%.2f,%.2f), fov=%.2f\n",
           scene->camera.position.x, scene->camera.position.y, scene->camera.position.z,
           scene->camera.orientation.x, scene->camera.orientation.y, scene->camera.orientation.z,
           scene->camera.fov);
    
    /* Print lights */
    printf("Lights (%d):\n", scene->num_lights);
    i = 0;
    while (i < scene->num_lights)
    {
        printf("  Light %d: pos=(%.2f,%.2f,%.2f), bright=%.2f, color=(%.2f,%.2f,%.2f)\n",
               i + 1,
               scene->lights[i].position.x, scene->lights[i].position.y, scene->lights[i].position.z,
               scene->lights[i].brightness,
               scene->lights[i].color.x, scene->lights[i].color.y, scene->lights[i].color.z);
        i++;
    }
    
    /* Print objects */
    printf("Objects (%d):\n", scene->num_objects);
    i = 0;
    while (i < scene->num_objects)
    {
        if (scene->objects[i].type == SPHERE)
        {
            sp = &scene->objects[i].data.sphere;
            printf("  Sphere %d: center=(%.2f,%.2f,%.2f), diam=%.2f, color=(%.2f,%.2f,%.2f)\n",
                   i + 1,
                   sp->center.x, sp->center.y, sp->center.z,
                   sp->diameter,
                   sp->color.x, sp->color.y, sp->color.z);
        }
        else if (scene->objects[i].type == PLANE)
        {
            pl = &scene->objects[i].data.plane;
            printf("  Plane %d: point=(%.2f,%.2f,%.2f), normal=(%.2f,%.2f,%.2f), color=(%.2f,%.2f,%.2f)\n",
                   i + 1,
                   pl->point.x, pl->point.y, pl->point.z,
                   pl->normal.x, pl->normal.y, pl->normal.z,
                   pl->color.x, pl->color.y, pl->color.z);
        }
        else if (scene->objects[i].type == CYLINDER)
        {
            cy = &scene->objects[i].data.cylinder;
            printf("  Cylinder %d: center=(%.2f,%.2f,%.2f), axis=(%.2f,%.2f,%.2f), diam=%.2f, height=%.2f, color=(%.2f,%.2f,%.2f)\n",
                   i + 1,
                   cy->center.x, cy->center.y, cy->center.z,
                   cy->axis.x, cy->axis.y, cy->axis.z,
                   cy->diameter, cy->height,
                   cy->color.x, cy->color.y, cy->color.z);
        }
        i++;
    }
}

/**
 * Main function
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

int main(int argc, char **argv)
{
    t_scene *scene;
    t_vars vars;

    if (argc != 2)
        error_exit(ERR_ARGS);
    scene = parse_scene_file(argv[1]);
    if (!scene)
        error_exit(ERR_SCENE);
    vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, WIDTH, HEIGHT, window_name_rt);
    create_image(&vars);
    int i =0;
    while (i < 100)
        put_pixel(&vars, i++, 50, 0xffffff);
    mlx_hooks(&vars);
    mlx_put_image_to_window(vars.mlx, vars.win, vars.img->img, 0, 0);
    mlx_loop(vars.mlx);
    free(scene);
    return 0;
}
