#ifndef RAYTRACE_H
# define RAYTRACE_H

# include "main.h"
# include "scene.h"
# include "math_utils.h"
# include <math.h>

/*
 * Ray structure for raytrace calculations
 */
typedef struct s_ray
{
	t_vec3		origin;
	t_vec3		direction;
}				t_ray;

/* Ray generation function */
t_ray	generate_camera_ray(const t_scene *scene, int x, int y);

/* Ray-object intersection functions */
double	hit_sphere(const t_sphere *sphere, t_ray ray);
double	hit_cylinder(const t_cylinder *cylinder, t_ray ray);

/* Main raytracing function */
int	trace_ray(const t_scene *scene, t_ray ray);

#endif
