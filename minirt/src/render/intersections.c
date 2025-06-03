#include "../includes/color_utils.h"
#include "../includes/intersections.h"
#include "../includes/math_utils.h"
#include "../includes/raytrace.h"
#include "../includes/scene.h"
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

/*
** Compute the quadratic coefficients for a ray-cylinder intersection.
** Returns a t_quadratic struct with the coefficients a, b, c.
*/
t_quadratic	cylinder_quadratic_coeffs(const t_cylinder *cylinder, t_ray ray)
{
	t_vec3		oc;
	t_vec3		ray_axis_cross;
	t_vec3		oc_axis_cross;
	double		radius;
	t_quadratic	q;

	oc = vec3_sub(ray.origin, cylinder->center);
	radius = cylinder->diameter / 2.0;
	ray_axis_cross = vec3_cross(ray.direction, cylinder->axis);
	oc_axis_cross = vec3_cross(oc, cylinder->axis);
	q.a = vec3_dot(ray_axis_cross, ray_axis_cross);
	q.b = 2.0 * vec3_dot(ray_axis_cross, oc_axis_cross);
	q.c = vec3_dot(oc_axis_cross, oc_axis_cross) - radius * radius;
	return (q);
}

/*
** Calculate the surface normal for a point on the cylinder.
*/
t_vec3	cylinder_surface_normal(const t_cylinder *cylinder, t_point3 point)
{
	double		m;
	t_vec3		axis_projection;
	t_point3	axis_point;

	m = vec3_dot(vec3_sub(point, cylinder->center), cylinder->axis);
	axis_projection = vec3_mult(cylinder->axis, m);
	axis_point = vec3_add(cylinder->center, axis_projection);
	return (vec3_normalize(vec3_sub(point, axis_point)));
}

/*
** Check intersection with cylinder cap (top or bottom)
** Returns 1 if hit, 0 if no hit
*/
int	intersect_cylinder_cap(const t_cylinder *cylinder, t_ray ray, t_hit *hit,
		double cap_height, int cap_side)
{
	double		denom;
	double		t;
	t_point3	cap_center;
	t_point3	intersection_point;
	t_vec3		to_intersection;
	double		distance_sq;
	double		radius_sq;

	denom = vec3_dot(cylinder->axis, ray.direction);
	if (fabs(denom) < 0.0001)
		return (0);
	cap_center = vec3_add(cylinder->center,
			vec3_mult(cylinder->axis, cap_height));
	t = vec3_dot(vec3_sub(cap_center, ray.origin), cylinder->axis) / denom;
	if (t <= 0.001)
		return (0);
	intersection_point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
	to_intersection = vec3_sub(intersection_point, cap_center);
	distance_sq = vec3_dot(to_intersection, to_intersection)
		- pow(vec3_dot(to_intersection, cylinder->axis), 2);
	radius_sq = pow(cylinder->diameter / 2.0, 2);
	if (distance_sq <= radius_sq)
	{
		if (hit->t < 0 || t < hit->t)
		{
			hit->t = t;
			hit->point = intersection_point;
			if (cap_side == 0)
				hit->normal = vec3_mult(cylinder->axis, -1);
			else
				hit->normal = cylinder->axis;
			hit->color = cylinder->material.color;
			hit->obj_type = CYLINDER;
			hit->hit_side = cap_side;
			return (1);
		}
	}
	return (0);
}

/*
** Calculate intersection with cylinder
** Returns 1 if hit, 0 if no hit
*/
int	intersect_cylinder(const t_cylinder *cylinder, t_ray ray, t_hit *hit)
{
	t_quadratic	q;
	double		t;
	double		m;
	t_point3	intersection_point;
	int			hit_found;

	hit_found = 0;
	/* ** Check intersection with cylinder side surface ** */
	q = cylinder_quadratic_coeffs(cylinder, ray);
	if (fabs(q.a) >= 0.0001)
	{
		t = solve_quadratic(q.a, q.b, q.c, 0.001);
		if (t > 0.001)
		{
			intersection_point = vec3_add(ray.origin,
					vec3_mult(ray.direction, t));
			m = vec3_dot(vec3_sub(intersection_point, cylinder->center),
					cylinder->axis);
			if (m >= 0 && m <= cylinder->height)
			{
				if (hit->t < 0 || t < hit->t)
				{
					hit->t = t;
					hit->point = intersection_point;
					hit->normal = cylinder_surface_normal(cylinder, hit->point);
					hit->color = cylinder->material.color;
					hit->obj_type = CYLINDER;
					hit->hit_side = 2;
					hit_found = 1;
				}
			}
		}
	}
	/* ** Always check both caps regardless of side surface hits ** */
	if (intersect_cylinder_cap(cylinder, ray, hit, 0, 0))
		hit_found = 1;
	if (intersect_cylinder_cap(cylinder, ray, hit, cylinder->height, 1))
		hit_found = 1;
	return (hit_found);
}

/*
** Calculate intersection with plane
** Returns 1 if hit, 0 if no hit
*/
int	intersect_plane(const t_plane *plane, t_ray ray, t_hit *hit)
{
	double	denom;
	double	t;
	t_vec3	oc;

	denom = vec3_dot(plane->normal, ray.direction);
	if (fabs(denom) < 0.0001)
		return (0);
	oc = vec3_sub(plane->point, ray.origin);
	t = vec3_dot(oc, plane->normal) / denom;
	if (t > 0.001)
	{
		if (hit->t < 0 || t < hit->t)
		{
			hit->t = t;
			hit->point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
			hit->normal = plane->normal;
			hit->color = plane->material.color;
			hit->obj_type = PLANE;
			hit->hit_side = -1;
			return (1);
		}
	}
	return (0);
}

/*
** Compute the quadratic coefficients for a ray-cone intersection.
** Returns a t_quadratic struct with the coefficients a, b, c.
*/
t_quadratic	cone_quadratic_coeffs(const t_cone *cone, t_ray ray)
{
	t_vec3		oc;
	double		cos_angle_sq;
	double		dv;
	double		ocv;
	t_quadratic	q;

	oc = vec3_sub(ray.origin, cone->vertex);
	cos_angle_sq = cos(cone->angle / 2.0) * cos(cone->angle / 2.0);
	dv = vec3_dot(ray.direction, cone->axis);
	ocv = vec3_dot(oc, cone->axis);
	q.a = dv * dv - cos_angle_sq;
	q.b = 2.0 * (dv * ocv - vec3_dot(ray.direction, oc) * cos_angle_sq);
	q.c = ocv * ocv - vec3_dot(oc, oc) * cos_angle_sq;
	return (q);
}

/*
** Calculate the surface normal for a point on the cone.
*/
t_vec3	cone_surface_normal(const t_cone *cone, t_point3 point)
{
	t_vec3	cone_to_point;
	double	projection_length;
	t_vec3	axis_projection;
	t_vec3	radial_vector;
	double	cos_angle;
	double	sin_angle;
	t_vec3	radial_component;
	t_vec3	axial_component;
	t_vec3	normal;

	cone_to_point = vec3_sub(point, cone->vertex);
	projection_length = vec3_dot(cone_to_point, cone->axis);
	axis_projection = vec3_mult(cone->axis, projection_length);
	radial_vector = vec3_sub(cone_to_point, axis_projection);
	cos_angle = cos(cone->angle / 2.0);
	sin_angle = sin(cone->angle / 2.0);
	radial_component = vec3_mult(vec3_normalize(radial_vector), cos_angle);
	axial_component = vec3_mult(cone->axis, -sin_angle);
	normal = vec3_add(radial_component, axial_component);
	return (vec3_normalize(normal));
}

/*
** Calculate intersection with cone base cap (circular base)
** Returns 1 if hit, 0 if no hit
*/
int	intersect_cone_cap(const t_cone *cone, t_ray ray, t_hit *hit)
{
	double		denom;
	double		t;
	t_point3	base_center;
	t_point3	intersection_point;
	t_vec3		to_intersection;
	double		distance_sq;
	double		radius_sq;

	/* ** Check intersection with base plane ** */
	denom = vec3_dot(cone->axis, ray.direction);
	if (fabs(denom) < 0.0001)
		return (0);
	base_center = vec3_add(cone->vertex, vec3_mult(cone->axis, cone->height));
	t = vec3_dot(vec3_sub(base_center, ray.origin), cone->axis) / denom;
	if (t <= 0.001)
		return (0);
	/* ** Check if intersection point is within circular base ** */
	intersection_point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
	to_intersection = vec3_sub(intersection_point, base_center);
	distance_sq = vec3_dot(to_intersection, to_intersection)
		- pow(vec3_dot(to_intersection, cone->axis), 2);
	radius_sq = pow(cone->height * tan(cone->angle / 2.0), 2);
	if (distance_sq <= radius_sq)
	{
		if (hit->t < 0 || t < hit->t)
		{
			hit->t = t;
			hit->point = intersection_point;
			hit->normal = cone->axis;
			hit->color = cone->material.color;
			hit->obj_type = CONE;
			hit->hit_side = 0;
			return (1);
		}
	}
	return (0);
}

/*
** Calculate intersection with cone
** Returns 1 if hit, 0 if no hit
*/
int	intersect_cone(const t_cone *cone, t_ray ray, t_hit *hit)
{
	t_quadratic	q;
	double		t;
	double		m;
	t_point3	intersection_point;
	int			hit_found;

	hit_found = 0;
	/* ** Check intersection with cone surface ** */
	q = cone_quadratic_coeffs(cone, ray);
	if (fabs(q.a) >= 0.0001)
	{
		t = solve_quadratic(q.a, q.b, q.c, 0.001);
		if (t > 0.001)
		{
			intersection_point = vec3_add(ray.origin,
					vec3_mult(ray.direction, t));
			m = vec3_dot(vec3_sub(intersection_point, cone->vertex),
					cone->axis);
			if (m >= 0 && m <= cone->height)
			{
				if (hit->t < 0 || t < hit->t)
				{
					hit->t = t;
					hit->point = intersection_point;
					hit->normal = cone_surface_normal(cone, hit->point);
					hit->color = cone->material.color;
					hit->obj_type = CONE;
					hit->hit_side = 1;
					hit_found = 1;
				}
			}
		}
	}
	/* ** Always check cone base cap regardless of surface hits ** */
	if (intersect_cone_cap(cone, ray, hit))
		hit_found = 1;
	return (hit_found);
}

/*
** Check intersection with all objects in scene
** Returns 1 if any hit, 0 if no hit
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
			{
				closest_hit->obj_index = i;
				hit_found = 1;
			}
		}
		else if (scene->objects[i].type == PLANE)
		{
			if (intersect_plane(&scene->objects[i].data.plane, ray,
					closest_hit))
			{
				closest_hit->obj_index = i;
				hit_found = 1;
			}
		}
		else if (scene->objects[i].type == CYLINDER)
		{
			if (intersect_cylinder(&scene->objects[i].data.cylinder, ray,
					closest_hit))
			{
				closest_hit->obj_index = i;
				hit_found = 1;
			}
		}
		else if (scene->objects[i].type == CONE)
		{
			if (intersect_cone(&scene->objects[i].data.cone, ray,
					closest_hit))
			{
				closest_hit->obj_index = i;
				hit_found = 1;
			}
		}
		i++;
	}
	return (hit_found);
}


