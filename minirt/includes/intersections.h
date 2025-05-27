#ifndef INTERSECTIONS_H
# define INTERSECTIONS_H

# include "raytrace.h"
# include "scene.h"
# include "math_utils.h"

/* 
 * Intersection structure to store hit info
 */
typedef struct s_hit
{
    double      t;          /* Distance along ray to intersection */
    t_vec3      point;      /* Point of intersection */
    t_vec3      normal;     /* Surface normal at intersection point */
    t_color3    color;      /* Object color */
    int         obj_type;   /* Type of the hit object */
    int         hit_side;   /* For cylinders: 0=bottom cap, 1=top cap, 2=side */
} t_hit;

/* 
 * Object intersection functions
 */
int     intersect_sphere(const t_sphere *sphere, t_ray ray, t_hit *hit);
int     intersect_plane(const t_plane *plane, t_ray ray, t_hit *hit);
int     intersect_cylinder(const t_cylinder *cylinder, t_ray ray, t_hit *hit);

/* 
 * Main intersection handler
 */
int     trace_objects(const t_scene *scene, t_ray ray, t_hit *closest_hit);
t_quadratic cylinder_quadratic_coeffs(const t_cylinder *cylinder, t_ray ray, t_vec3 *d_perp, t_vec3 *oc_perp);
t_quadratic sphere_quadratic_coeffs(const t_sphere *sphere, t_ray ray);
int     cylinder_height_check(const t_cylinder *cylinder, t_ray ray, double t);
double  hit_cylinder_cap(const t_cylinder *cylinder, t_ray ray, int is_top);
double  hit_cylinder_with_type(const t_cylinder *cylinder, t_ray ray, int *hit_type);

#endif
