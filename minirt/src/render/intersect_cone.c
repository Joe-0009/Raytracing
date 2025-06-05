#include "../includes/minirt_app.h"
#include "../includes/scene_math.h"
#include <math.h>

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
	t_vec3	axis_projection;
	t_vec3	radial_vector;
	double	cos_angle;

	cone_to_point = vec3_sub(point, cone->vertex);
	axis_projection = vec3_mult(cone->axis, vec3_dot(cone_to_point,
				cone->axis));
	radial_vector = vec3_sub(cone_to_point, axis_projection);
	cos_angle = cos(cone->angle / 2.0);
	return (vec3_normalize(vec3_add(vec3_mult(vec3_normalize(radial_vector),
					cos_angle), vec3_mult(cone->axis, -sin(cone->angle
						/ 2.0)))));
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
	t_point3	point;

	denom = vec3_dot(cone->axis, ray.direction);
	if (fabs(denom) < 0.0001)
		return (0);
	base_center = vec3_add(cone->vertex, vec3_mult(cone->axis, cone->height));
	t = vec3_dot(vec3_sub(base_center, ray.origin), cone->axis) / denom;
	if (t <= 0.001 || (hit->t >= 0 && t >= hit->t))
		return (0);
	point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
	if (vec3_length_squared(vec3_sub(vec3_sub(point, base_center),
				vec3_mult(cone->axis, vec3_dot(vec3_sub(point, base_center),
						cone->axis)))) > pow(cone->height * tan(cone->angle
				/ 2.0), 2))
		return (0);
	hit->t = t;
	hit->point = point;
	hit->normal = cone->axis;
	hit->color = cone->material.color;
	hit->obj_type = CONE;
	hit->hit_side = 0;
	return (1);
}

/*
** Check intersection with cone surface and update hit if valid
*/
static int	check_cone_surface(const t_cone *cone, t_ray ray, t_hit *hit)
{
	t_quadratic	q;
	double		t;
	double		m;
	t_point3	intersection_point;

	q = cone_quadratic_coeffs(cone, ray);
	if (fabs(q.a) < 0.0001)
		return (0);
	t = solve_quadratic(q.a, q.b, q.c, 0.001);
	if (t <= 0.001)
		return (0);
	intersection_point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
	m = vec3_dot(vec3_sub(intersection_point, cone->vertex), cone->axis);
	if (m < 0 || m > cone->height)
		return (0);
	if (hit->t >= 0 && t >= hit->t)
		return (0);
	hit->t = t;
	hit->point = intersection_point;
	hit->normal = cone_surface_normal(cone, hit->point);
	hit->color = cone->material.color;
	hit->obj_type = CONE;
	hit->hit_side = 1;
	return (1);
}

/*
** Calculate intersection with cone
** Returns 1 if hit, 0 if no hit
*/
int	intersect_cone(const t_cone *cone, t_ray ray, t_hit *hit)
{
	int	hit_found;

	hit_found = 0;
	if (check_cone_surface(cone, ray, hit))
		hit_found = 1;
	if (intersect_cone_cap(cone, ray, hit))
		hit_found = 1;
	return (hit_found);
}
