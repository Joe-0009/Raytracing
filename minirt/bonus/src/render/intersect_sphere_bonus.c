#include "../includes/minirt_app_bonus.h"
#include "../includes/scene_bonus.h"
#include <math.h>

/*
** Compute quadratic coefficients for ray-sphere intersection
** Returns coefficients in t_quadratic struct
*/
static t_quadratic	sphere_quadratic_coeffs(const t_sphere *sphere, t_ray ray)
{
	t_quadratic	coeffs;
	t_vec3		oc;

	oc = vec3_sub(ray.origin, sphere->center);
	coeffs.a = vec3_dot(ray.direction, ray.direction);
	coeffs.b = 2.0 * vec3_dot(oc, ray.direction);
	coeffs.c = vec3_dot(oc, oc) - sphere->radius * sphere->radius;
	return (coeffs);
}

/*
** Check intersection between ray and sphere
** Returns 1 if hit, 0 if no hit
*/
int	intersect_sphere(const t_sphere *sphere, t_ray ray, t_hit *hit)
{
	t_quadratic	coeffs;
	double		t;
	t_vec3		center_to_hit;
	t_vec3		tangent, bitangent;

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
	center_to_hit = vec3_sub(hit->point, sphere->center);
	hit->normal = vec3_normalize(center_to_hit);
	hit->uv = sphere_uv_mapping(hit->normal);
	if (sphere->bump.has_bump_map)
	{
		tangent = vec3_create(-hit->normal.z, 0, hit->normal.x);
		if (vec3_length(tangent) < 0.001)
			tangent = vec3_create(1, 0, 0);
		else
			tangent = vec3_normalize(tangent);
		bitangent = vec3_normalize(vec3_cross(hit->normal, tangent));
		hit->normal = apply_bump_mapping(hit->normal, hit->uv, &sphere->bump,
				tangent, bitangent);
	}
	if (sphere->texture.has_texture)
		hit->color = sample_texture(&sphere->texture, hit->uv);
	else
		hit->color = sphere->color;
	hit->obj_type = SPHERE;
	hit->hit_side = -1;
	if (vec3_dot(ray.direction, hit->normal) > 0.0)
		hit->normal = vec3_mult(hit->normal, -1.0);
	return (1);
}
