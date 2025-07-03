#include "../includes/minirt_app.h"
#include "../includes/scene_math.h"
#include <math.h>

/*
** Compute quadratic coefficients for ray-sphere intersection
** Returns coefficients in t_quadratic struct
*/
t_quadratic	sphere_quadratic_coeffs(const t_sphere *sphere, t_ray ray)
{
	t_quadratic	coeffs;
	t_vec3		oc;
	double		radius;

	oc = vec3_sub(ray.origin, sphere->center);
	radius = sphere->diameter / 2.0;
	coeffs.a = vec3_dot(ray.direction, ray.direction);
	coeffs.b = 2.0 * vec3_dot(oc, ray.direction);
	coeffs.c = vec3_dot(oc, oc) - radius * radius;
	return (coeffs);
}

/*
** Check intersection between ray and sphere
** Returns 1 if hit, 0 if no hit
** Unified implementation - no redundant calculations
*/
int	intersect_sphere(const t_sphere *sphere, t_ray ray, t_hit *hit)
{
	t_quadratic	coeffs;
	double		t;

	coeffs = sphere_quadratic_coeffs(sphere, ray);
	if (coeffs.b * coeffs.b < 4.0 * coeffs.a * coeffs.c)
		return (0);
	t = solve_quadratic(coeffs.a, coeffs.b, coeffs.c, 0.001);
	if (t < 0.0)
		return (0);
	if (hit->t > 0.0 && t >= hit->t)
		return (0);
	hit->t = t;
	hit->point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
	hit->normal = vec3_normalize(vec3_sub(hit->point, sphere->center));
	hit->color = sphere->material.color;
	hit->obj_type = SPHERE;
	hit->hit_side = -1;
	if (vec3_dot(ray.direction, hit->normal) > 0.0)
		hit->normal = vec3_mult(hit->normal, -1.0);
	return (1);
}
