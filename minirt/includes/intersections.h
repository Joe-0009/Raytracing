#ifndef INTERSECTIONS_H
# define INTERSECTIONS_H

# include "scene_math.h"

typedef struct s_hit
{
	double		t;
	t_vec3		point;
	t_vec3		normal;
	t_color3	color;
	int			obj_type;
	int			obj_index;
	int			hit_side;
} t_hit;

int		intersect_sphere(const t_sphere *sphere, t_ray ray, t_hit *hit);
int		intersect_plane(const t_plane *plane, t_ray ray, t_hit *hit);
int		intersect_cylinder(const t_cylinder *cylinder, t_ray ray, t_hit *hit);
int		intersect_cone(const t_cone *cone, t_ray ray, t_hit *hit);
int		trace_objects(const t_scene *scene, t_ray ray, t_hit *closest_hit);
t_quadratic	sphere_quadratic_coeffs(const t_sphere *sphere, t_ray ray);
t_quadratic	cylinder_quadratic_coeffs(const t_cylinder *cylinder, t_ray ray);
t_quadratic	cone_quadratic_coeffs(const t_cone *cone, t_ray ray);
t_vec3		cylinder_surface_normal(const t_cylinder *cylinder, t_point3 point);
t_vec3		cone_surface_normal(const t_cone *cone, t_point3 point);
int			intersect_cone_cap(const t_cone *cone, t_ray ray, t_hit *hit);

#endif
