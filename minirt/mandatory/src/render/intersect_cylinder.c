#include "../includes/minirt_app.h"
#include "../includes/scene_math.h"
#include <math.h>

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
** Set hit data for cylinder cap
*/
static void	set_cap_hit_data(t_hit *hit, const t_cylinder *cyl, int is_top_cap)
{
	hit->color = cyl->material.color;
	hit->obj_type = CYLINDER;
	if (is_top_cap)
	{
		hit->normal = cyl->axis;
		hit->hit_side = 1;
	}
	else
	{
		hit->normal = vec3_mult(cyl->axis, -1);
		hit->hit_side = 0;
	}
}

/*
** Check intersection with cylinder cap (top or bottom)
** Returns 1 if hit, 0 if no hit
*/
static int	check_cap_hit(const t_cylinder *cyl, t_ray ray, t_hit *hit,
		double height)
{
	double		denom;
	double		t;
	t_point3	cap_center;
	t_point3	point;
	t_vec3		radial;

	denom = vec3_dot(cyl->axis, ray.direction);
	if (fabs(denom) < 0.0001)
		return (0);
	cap_center = vec3_add(cyl->center, vec3_mult(cyl->axis, height));
	t = vec3_dot(vec3_sub(cap_center, ray.origin), cyl->axis) / denom;
	if (t <= 0.001 || (hit->t >= 0 && t >= hit->t))
		return (0);
	point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
	radial = vec3_sub(point, cap_center);
	radial = vec3_sub(radial, vec3_mult(cyl->axis, vec3_dot(radial,
					cyl->axis)));
	if (vec3_length(radial) > cyl->diameter / 2.0)
		return (0);
	hit->t = t;
	hit->point = point;
	set_cap_hit_data(hit, cyl, (height > 0));
	return (1);
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
	t_point3	point;

	if (check_cap_hit(cylinder, ray, hit, 0) || check_cap_hit(cylinder, ray,
			hit, cylinder->height))
		return (1);
	q = cylinder_quadratic_coeffs(cylinder, ray);
	if (fabs(q.a) < 0.0001)
		return (0);
	t = solve_quadratic(q.a, q.b, q.c, 0.001);
	if (t <= 0.001 || (hit->t >= 0 && t >= hit->t))
		return (0);
	point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
	m = vec3_dot(vec3_sub(point, cylinder->center), cylinder->axis);
	if (m < 0 || m > cylinder->height)
		return (0);
	hit->t = t;
	hit->point = point;
	hit->normal = cylinder_surface_normal(cylinder, point);
	hit->color = cylinder->material.color;
	hit->obj_type = CYLINDER;
	hit->hit_side = 2;
	return (1);
}
