#include "../includes/minirt_app.h"
#include "../includes/scene_math.h"
#include <math.h>

/*
** Compute the quadratic coefficients for a ray-sphere intersection.
** Returns a t_quadratic struct with the coefficients a, b, c.
*/
t_quadratic	sphere_quadratic_coeffs(const t_sphere *sphere, t_ray ray)
{
	t_vec3		oc;
	double		r;
	t_quadratic	q;

	oc = vec3_sub(ray.origin, sphere->center);
	r = sphere->diameter / 2.0;
	q.a = vec3_dot(ray.direction, ray.direction);
	q.b = 2.0 * vec3_dot(oc, ray.direction);
	q.c = vec3_dot(oc, oc) - r * r;
	return (q);
}

/*
** Calculate intersection with sphere
** Returns 1 if hit, 0 if no hit
*/
int	intersect_sphere(const t_sphere *sphere, t_ray ray, t_hit *hit)
{
	t_quadratic	q;
	double		t;

	q = sphere_quadratic_coeffs(sphere, ray);
	t = solve_quadratic(q.a, q.b, q.c, 0.001);
	if (t > 0.001)
	{
		if (hit->t < 0 || t < hit->t)
		{
			hit->t = t;
			hit->point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
			hit->normal = vec3_normalize(vec3_sub(hit->point, sphere->center));
			hit->color = sphere->material.color;
			hit->obj_type = SPHERE;
			hit->hit_side = -1;
			return (1);
		}
	}
	return (0);
}
