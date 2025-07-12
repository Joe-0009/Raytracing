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
** Calculate basic hit information for a sphere
** Returns 1 if hit is successful, 0 otherwise
*/
static int	calculate_sphere_hit(const t_sphere *sphere, t_ray ray, t_hit *hit)
{
	t_quadratic	coeffs;
	double		t;
	t_vec3		center_to_hit;

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
	return (1);
}

/*
** Apply bump mapping to the sphere normal if enabled
*/
static t_vec3	apply_sphere_bump(const t_sphere *sphere, t_hit *hit)
{
	t_vec3	tangent;
	t_vec3	bitangent;

	if (!sphere->bump.is_active)
		return (hit->normal);
	tangent = vec3_create(-hit->normal.z, 0, hit->normal.x);
	if (vec3_length(tangent) < 0.001)
		tangent = vec3_create(1, 0, 0);
	else
		tangent = vec3_normalize(tangent);
	bitangent = vec3_normalize(vec3_cross(hit->normal, tangent));
	return (apply_bump_mapping(hit->normal, hit->uv, &sphere->bump, tangent,
			bitangent));
}

/*
** Determine the color at hit point based on texture, checkerboard,
	or solid color
*/
static t_vec3	determine_sphere_color(const t_sphere *sphere, t_hit *hit)
{
	int	check_u;
	int	check_v;

	if (sphere->texture.is_active)
		return (sample_texture(&sphere->texture, hit->uv));
	else if (sphere->checkerboard)
	{
		check_u = (int)floor(hit->uv.u * 8.0);
		check_v = (int)floor(hit->uv.v * 8.0);
		if ((check_u + check_v) % 2 == 0)
			return (vec3_create(1.0, 1.0, 1.0));
		else
			return (vec3_create(0.0, 0.0, 0.0));
	}
	return (sphere->color);
}

/*
** Check intersection between ray and sphere
** Returns 1 if hit, 0 if no hit
*/
int	intersect_sphere(const t_sphere *sphere, t_ray ray, t_hit *hit)
{
	if (!calculate_sphere_hit(sphere, ray, hit))
		return (0);
	hit->normal = apply_sphere_bump(sphere, hit);
	hit->color = determine_sphere_color(sphere, hit);
	hit->obj_type = SPHERE;
	hit->hit_side = -1;
	if (vec3_dot(ray.direction, hit->normal) > 0.0)
		hit->normal = vec3_mult(hit->normal, -1.0);
	return (1);
}
