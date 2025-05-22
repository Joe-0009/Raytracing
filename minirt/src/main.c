#include "../includes/minirt.h"

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
int main(int argc, char **argv)
{
    t_scene *scene;
    
    if (argc != 2)
        error_exit(ERR_ARGS);
    scene = parse_scene_file_combined(argv[1]);
    if (!scene)
        error_exit(ERR_SCENE);
    
    /* Print scene information for debugging */
    print_scene_info(scene);
    
    /* In a real program, we would render the scene here */
    printf("\nScene parsed successfully. Rendering functionality will be implemented later.\n");
    
    /* Cleanup */
    free(scene);
    
    return 0;
}
