#ifndef RAYTRACE_H
# define RAYTRACE_H

# include "main.h"
# include "scene.h"
# include "math_utils.h"
# include <math.h>

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
} t_ray;

t_ray	generate_camera_ray(const t_scene *scene, int x, int y);
int		trace_ray(const t_scene *scene, t_ray ray);

#endif
