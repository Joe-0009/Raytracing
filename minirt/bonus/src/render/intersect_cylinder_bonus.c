#include "../includes/minirt_app_bonus.h"
#include "../includes/scene_bonus.h"
#include <math.h>

static t_vec3	cylinder_surface_normal(const t_cylinder *cylinder,
		t_point3 point)
{
	double		m;
	t_vec3		axis_projection;
	t_point3	axis_point;

	m = vec3_dot(vec3_sub(point, cylinder->center), cylinder->axis);
	axis_projection = vec3_mult(cylinder->axis, m);
	axis_point = vec3_add(cylinder->center, axis_projection);
	return (vec3_normalize(vec3_sub(point, axis_point)));
}

static void	set_cap_hit_data(t_hit *hit, const t_cylinder *cyl, int is_top_cap)
{
	hit->color = cyl->color;
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
static int	check_cylinder_surface(const t_cylinder *cylinder, t_ray ray,
		t_hit *hit, double t)
{
	t_point3	point;
	double		m;

	point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
	m = vec3_dot(vec3_sub(point, cylinder->center), cylinder->axis);
	if (m < 0 || m > cylinder->height)
		return (0);
	hit->t = t;
	hit->point = point;
	hit->normal = cylinder_surface_normal(cylinder, point);
	hit->color = cylinder->color;
	hit->obj_type = CYLINDER;
	hit->hit_side = 2;
	return (1);
}

int	intersect_cylinder(const t_cylinder *cylinder, t_ray ray, t_hit *hit)
{
	t_quadratic	q;
	double		t;
	int			top_cap_hit;
	int			bottom_cap_hit;
	int			cap_hit;

	bottom_cap_hit = check_cap_hit(cylinder, ray, hit, 0);
	top_cap_hit = check_cap_hit(cylinder, ray, hit, cylinder->height);
	cap_hit = bottom_cap_hit || top_cap_hit;
	q = cylinder_quadratic_coeffs(cylinder, ray);
	if (fabs(q.a) < 0.0001)
		return (cap_hit);
	t = solve_quadratic(q.a, q.b, q.c, 0.001);
	if (t <= 0.001 || (cap_hit && t >= hit->t))
		return (cap_hit);
	if (!check_cylinder_surface(cylinder, ray, hit, t))
		return (cap_hit);
	return (1);
}
