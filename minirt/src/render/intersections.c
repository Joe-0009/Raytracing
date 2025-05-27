#include "../includes/color_utils.h"
#include "../includes/intersections.h"
#include "../includes/math_utils.h"
#include "../includes/raytrace.h"
#include "../includes/scene.h"
#include <math.h>

// Color functions moved to color_utils.c

/*
 * Calculate the normal for a cylinder hit based on hit type
 */
static t_vec3	calculate_cylinder_normal(const t_cylinder *cylinder, t_vec3 hit_point, int hit_type)
{
	t_vec3	to_point;
	t_vec3	proj_on_axis;

	if (hit_type == 0) // bottom cap
		return (vec3_mult(cylinder->axis, -1.0));
	else if (hit_type == 1) // top cap
		return (cylinder->axis);
	else // side hit (hit_type == 2)
	{
		to_point = vec3_sub(hit_point, cylinder->center);
		proj_on_axis = vec3_mult(cylinder->axis, vec3_dot(to_point, cylinder->axis));
		return (vec3_normalize(vec3_sub(to_point, proj_on_axis)));
	}
}

/*
 * Calculate intersection with sphere
 * Returns 1 if hit, 0 if no hit
 */
int	intersect_sphere(const t_sphere *sphere, t_ray ray, t_hit *hit)
{
	double	t;

	t = hit_sphere(sphere, ray);
	if (t > 0.001 && (hit->t < 0 || t < hit->t))
	{
		hit->t = t;
		hit->point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
		hit->normal = vec3_normalize(vec3_sub(hit->point, sphere->center));
		hit->color = sphere->color;
		hit->obj_type = SPHERE;
		hit->hit_side = -1;
		return (1);
	}
	return (0);
}

/*
 * Calculate intersection with cylinder
 * Returns 1 if hit, 0 if no hit
 */
int	intersect_cylinder(const t_cylinder *cylinder, t_ray ray, t_hit *hit)
{
	double	t;
	int		hit_type;

	t = hit_cylinder_with_type(cylinder, ray, &hit_type);
	if (t > 0.001 && (hit->t < 0 || t < hit->t))
	{
		hit->t = t;
		hit->point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
		hit->hit_side = hit_type;
		hit->normal = calculate_cylinder_normal(cylinder, hit->point, hit_type);
		hit->color = cylinder->color;
		hit->obj_type = CYLINDER;
		return (1);
	}
	return (0);
}

/*
 * Calculate intersection with plane
 * Returns 1 if hit, 0 if no hit
 */
int	intersect_plane(const t_plane *plane, t_ray ray, t_hit *hit)
{
	double	denom;
	double	t;

	denom = vec3_dot(ray.direction, plane->normal);
	if (fabs(denom) < 1e-6)
		return (0);
	t = vec3_dot(vec3_sub(plane->point, ray.origin), plane->normal) / denom;
	if (t < 0.001 || (hit->t >= 0 && t >= hit->t))
		return (0);
	hit->t = t;
	hit->point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
	hit->normal = plane->normal;
	hit->color = plane->color;
	hit->obj_type = PLANE;
	hit->hit_side = -1;
	return (1);
}

/*
 * Check intersection with all objects in scene
 * Returns 1 if any hit, 0 if no hit
 */
int	trace_objects(const t_scene *scene, t_ray ray, t_hit *closest_hit)
{
	int	i;
	int	hit_found;

	closest_hit->t = -1;
	hit_found = 0;
	i = 0;
	while (i < scene->num_objects)
	{
		if (scene->objects[i].type == SPHERE)
		{
			if (intersect_sphere(&scene->objects[i].data.sphere, ray,
					closest_hit))
				hit_found = 1;
		}
		else if (scene->objects[i].type == PLANE)
		{
			if (intersect_plane(&scene->objects[i].data.plane, ray,
					closest_hit))
				hit_found = 1;
		}
		else if (scene->objects[i].type == CYLINDER)
		{
			if (intersect_cylinder(&scene->objects[i].data.cylinder, ray,
					closest_hit))
				hit_found = 1;
		}
		i++;
	}
	return (hit_found);
}

/*
 * Compute the quadratic coefficients for a ray-cylinder intersection.
 * d_perp and oc_perp are the perpendicular components to the cylinder axis.
 * Returns a t_quadratic struct with the coefficients a, b, c.
 */
t_quadratic	cylinder_quadratic_coeffs(const t_cylinder *cylinder, t_ray ray,
		t_vec3 *d_perp, t_vec3 *oc_perp)
{
	t_vec3		oc;
	double		axis_dot_d;
	double		axis_dot_oc;
	t_quadratic	q;

	oc = vec3_sub(ray.origin, cylinder->center);
	axis_dot_d = vec3_dot(ray.direction, cylinder->axis);
	axis_dot_oc = vec3_dot(oc, cylinder->axis);
	*d_perp = vec3_sub(ray.direction, vec3_mult(cylinder->axis, axis_dot_d));
	*oc_perp = vec3_sub(oc, vec3_mult(cylinder->axis, axis_dot_oc));
	q.a = vec3_dot(*d_perp, *d_perp);
	q.b = 2.0 * vec3_dot(*d_perp, *oc_perp);
	q.c = vec3_dot(*oc_perp, *oc_perp) - (cylinder->diameter / 2.0)
		* (cylinder->diameter / 2.0);
	return (q);
}

/*
 * Check if the intersection point is within the finite height of the cylinder.
 * Returns 1 if within height, 0 otherwise.
 */
int	cylinder_height_check(const t_cylinder *cylinder, t_ray ray, double t)
{
	t_vec3	hit_point;
	double	height_proj;

	hit_point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
	height_proj = vec3_dot(vec3_sub(hit_point, cylinder->center),
			cylinder->axis);
	return (height_proj >= 0 && height_proj <= cylinder->height);
}

/*
 * Compute the quadratic coefficients for a ray-sphere intersection.
 * Returns a t_quadratic struct with the coefficients a, b, c.
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
