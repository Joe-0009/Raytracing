#include "../includes/minirt_app_bonus.h"
#include "../includes/scene_bonus.h"
#include <math.h>

t_cone_constants	get_cone_constants(const t_cone *cone)
{
	t_cone_constants	constants;
	double				half_angle;

	half_angle = cone->angle / 2.0;
	constants.cos_angle = cos(half_angle);
	constants.sin_angle = sin(half_angle);
	constants.cos_angle_sq = constants.cos_angle * constants.cos_angle;
	constants.tan_half_angle = tan(half_angle);
	return (constants);
}

t_vec3	cone_surface_normal(const t_cone *cone, t_point3 point)
{
	t_vec3				cone_to_point;
	t_vec3				axis_projection;
	t_vec3				radial_vector;
	t_cone_constants	constants;

	constants = get_cone_constants(cone);
	cone_to_point = vec3_sub(point, cone->vertex);
	axis_projection = vec3_mult(cone->axis, vec3_dot(cone_to_point,
				cone->axis));
	radial_vector = vec3_sub(cone_to_point, axis_projection);
	return (vec3_normalize(vec3_add(vec3_mult(vec3_normalize(radial_vector),
					constants.cos_angle), vec3_mult(cone->axis,
					-constants.sin_angle))));
}

static int	intersect_cone_cap(const t_cone *cone, t_ray ray, t_hit *hit)
{
	t_cone_constants	constants;

	t_point (point), (base_center);
	double (denom), (t), (cap_radius_sq);
	denom = vec3_dot(cone->axis, ray.direction);
	if (fabs(denom) < EPSILON)
		return (0);
	base_center = vec3_add(cone->vertex, vec3_mult(cone->axis, cone->height));
	t = vec3_dot(vec3_sub(base_center, ray.origin), cone->axis) / denom;
	if (t <= MIN_T || (hit->t >= 0 && t >= hit->t))
		return (0);
	point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
	constants = get_cone_constants(cone);
	cap_radius_sq = pow(cone->height * constants.tan_half_angle, 2);
	if (vec3_length_squared(vec3_sub(vec3_sub(point, base_center),
				vec3_mult(cone->axis, vec3_dot(vec3_sub(point, base_center),
						cone->axis)))) > cap_radius_sq)
		return (0);
	hit->t = t;
	hit->point = point;
	hit->normal = cone->axis;
	hit->color = cone->color;
	hit->obj_type = CONE;
	hit->hit_side = 0;
	return (1);
}

static int	check_cone_surface(const t_cone *cone, t_ray ray, t_hit *hit)
{
	t_quadratic	q;
	double		t;
	double		m;
	t_point3	intersection_point;

	q = cone_quadratic_coeffs(cone, ray);
	if (fabs(q.a) < EPSILON)
		return (0);
	t = solve_quadratic(q.a, q.b, q.c, MIN_T);
	if (t <= MIN_T)
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
	hit->color = cone->color;
	hit->obj_type = CONE;
	hit->hit_side = 1;
	return (1);
}

int	intersect_cone(const t_cone *cone, t_ray ray, t_hit *hit)
{
	int		surface_hit;
	int		cap_hit;
	t_hit	temp_hit;
	int		hit_found;

	hit_found = 0;
	surface_hit = check_cone_surface(cone, ray, hit);
	if (surface_hit)
		hit_found = 1;
	temp_hit.t = -1.0;
	cap_hit = intersect_cone_cap(cone, ray, &temp_hit);
	if (cap_hit && (temp_hit.t < hit->t || hit->t < 0))
	{
		*hit = temp_hit;
		hit_found = 1;
	}
	return (hit_found);
}
