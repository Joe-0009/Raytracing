#include "../includes/color_utils.h"
#include "../includes/intersections.h"
#include "../includes/math_utils.h"
#include "../includes/raytrace.h"
#include "../includes/scene.h"
#include <math.h>

// Color functions moved to color_utils.c

/*
 * Calculate intersection with sphere
 * Returns 1 if hit, 0 if no hit
 */
int	intersect_sphere(const t_sphere *sphere, t_ray ray, t_hit *hit)
{
	double	t;

	t = hit_sphere(sphere, ray);
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

/*
 * Calculate intersection with cylinder
 * Returns 1 if hit, 0 if no hit
 * NOTE: Cylinder rendering is disabled - this function always returns 0
 */
int	intersect_cylinder(const t_cylinder *cylinder, t_ray ray, t_hit *hit)
{
	// Cylinder rendering disabled
	(void)cylinder;
	(void)ray;
	(void)hit;
	return (0);
}

/*
 * Calculate intersection with plane
 * Returns 1 if hit, 0 if no hit
 * NOTE: Plane rendering is disabled - this function always returns 0
 */
int intersect_plane(const t_plane *plane, t_ray ray, t_hit *hit)
{
	// Plane rendering disabled
	(void)plane;
	(void)ray;
	(void)hit;
	return (0);
}

/*
 * Calculate intersection with cone
 * Returns 1 if hit, 0 if no hit
 * NOTE: cone rendering is disabled - this function always returns 0
 */
int intersect_cone(const t_cone *cone, t_ray ray, t_hit *hit)
{
	// Plane rendering disabled
	(void)cone;
	(void)ray;
	(void)hit;
	return (0);
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
		else if (scene->objects[i].type == CONE)
		{
			if (intersect_cone(&scene->objects[i].data.cone, ray,
					closest_hit))
				hit_found = 1;
		}
		i++;
	}
	return (hit_found);
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
