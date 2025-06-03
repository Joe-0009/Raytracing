#ifndef INTERSECTIONS_H
# define INTERSECTIONS_H

# include "math_utils.h"
# include "raytrace.h"
# include "scene.h"

/*
 * Intersection structure to store hit info
 */
typedef struct s_hit
{
	double t;       /* Distance along ray to intersection */
	t_vec3 point;   /* Point of intersection */
	t_vec3 normal;  /* Surface normal at intersection point */
	t_color3 color; /* Object color */
	int obj_type;   /* Type of the hit object */
	int obj_index;  /* Index of the hit object in scene array */
	int hit_side;   /* For cylinders: 0=bottom cap, 1=top cap, 2=side */
}			t_hit;

/*
 * Object intersection functions
 */
int			intersect_sphere(const t_sphere *sphere, t_ray ray, t_hit *hit);
int			intersect_plane(const t_plane *plane, t_ray ray, t_hit *hit);
int			intersect_cylinder(const t_cylinder *cylinder, t_ray ray,
				t_hit *hit);
int			intersect_cone(const t_cone *cone, t_ray ray, t_hit *hit);

/*
 * Main intersection handler
 */
int			trace_objects(const t_scene *scene, t_ray ray, t_hit *closest_hit);

/*
 * Helper functions for quadratic coefficients
 */
t_quadratic	sphere_quadratic_coeffs(const t_sphere *sphere, t_ray ray);
t_quadratic	cylinder_quadratic_coeffs(const t_cylinder *cylinder, t_ray ray);
t_quadratic	cone_quadratic_coeffs(const t_cone *cone, t_ray ray);

/*
 * Helper functions for surface normals
 */
t_vec3		cylinder_surface_normal(const t_cylinder *cylinder, t_point3 point);
t_vec3		cone_surface_normal(const t_cone *cone, t_point3 point);

/*
 * Helper functions for caps
 */
int			intersect_cone_cap(const t_cone *cone, t_ray ray, t_hit *hit);

#endif
