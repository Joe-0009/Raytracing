#include "../includes/minirt.h"
#include "../includes/events.h"
#include "../includes/main.h"
#include "../includes/draw.h"
#include <stdio.h>

t_scene	*g_scene = NULL;

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
    t_cone *cn;
    
    printf("Scene Information:\n");
    
    /* Print ambient lighting */
    printf("Ambient: ratio=%.2f, color=(%.0f,%.0f,%.0f)\n",
           scene->ambient.ratio,
           scene->ambient.color.x * 255, scene->ambient.color.y * 255, scene->ambient.color.z * 255);
    
    /* Print light source */
    printf("Light: pos=(%.2f,%.2f,%.2f), brightness=%.2f, color=(%.0f,%.0f,%.0f)\n",
           scene->light.position.x, scene->light.position.y, scene->light.position.z,
           scene->light.brightness,
           scene->light.color.x * 255, scene->light.color.y * 255, scene->light.color.z * 255);
    
    /* Print camera */
    printf("Camera: pos=(%.2f,%.2f,%.2f), dir=(%.2f,%.2f,%.2f), fov=%.2f\n",
           scene->camera.position.x, scene->camera.position.y, scene->camera.position.z,
           scene->camera.orientation.x, scene->camera.orientation.y, scene->camera.orientation.z,
           scene->camera.fov);
    
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
        else if (scene->objects[i].type == CONE)
        {
            cn = &scene->objects[i].data.cone;
            printf("  Cone %d: vertex=(%.2f,%.2f,%.2f), axis=(%.2f,%.2f,%.2f), angle=%.2f, height=%.2f, color=(%.2f,%.2f,%.2f)\n",
                   i + 1,
                   cn->vertex.x, cn->vertex.y, cn->vertex.z,
                   cn->axis.x, cn->axis.y, cn->axis.z,
                   cn->angle * 180.0 / M_PI, // Convert radians to degrees for display
                   cn->height,
                   cn->color.x, cn->color.y, cn->color.z);
        }
        i++;
    }
}

/* ** Set global scene for transformations ** */
void	set_scene_for_transforms(t_scene *scene)
{
	g_scene = scene;
}

/*
** Print controls help
*/
void	print_controls_help(void)
{
	printf("\n=== MiniRT Transform Controls ===\n");
	printf("CAMERA MOVEMENT:\n");
	printf("  W/S - Move forward/backward\n");
	printf("  A/D - Move left/right\n");
	printf("  Q/E - Move down/up\n");
	printf("\nCAMERA ROTATION:\n");
	printf("  I/K - Look up/down\n");
	printf("  J/L - Look left/right\n");
	printf("\nOBJECT CONTROLS:\n");
	printf("  P/O - Select object (next/previous)\n");
	printf("  Arrow keys - Move object (left/right/up/down)\n");
	printf("  +/- - Scale object up/down\n");
	printf("  R/F - Rotate object around X-axis (forward/reverse)\n");
	printf("  T/G - Rotate object around Y-axis (forward/reverse)\n");
	printf("\nOTHER:\n");
	printf("  SPACE - Show this help\n");
	printf("  ESC - Exit\n");
	printf("==============================\n\n");
}

/**
 * Main function
 */


int main(int argc, char **argv)
{
    t_scene *scene;
    t_vars vars;

    if (argc != 2)
    {
        error_exit(ERR_ARGS);
    }
    
    scene = parse_scene_file(argv[1]);
    if (!scene)
    {
        error_exit(ERR_SCENE);
    }
    
    print_scene_info(scene);
    
    vars.mlx = mlx_init();
    if (!vars.mlx)
    {
        error_exit("Error: MLX initialization failed\n");
    }
    
    vars.win = mlx_new_window(vars.mlx, WIDTH, HEIGHT, "miniRT");
    if (!vars.win)
    {
        error_exit("Error: Window creation failed\n");
    }
    
    create_image(&vars);
    
    /* ** Initialize transformation system ** */
    set_scene_for_transforms(scene);
    print_controls_help();
    
    main_draw(&vars, scene);
    
    mlx_hooks(&vars);
    
    mlx_put_image_to_window(vars.mlx, vars.win, vars.img->img, 0, 0);
    
    mlx_loop(vars.mlx);
    
    free(scene);
    return 0;
}
