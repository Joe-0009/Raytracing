#include "../includes/minirt_app_bonus.h"
#include "../includes/scene_bonus.h"
#include <math.h>


t_quadratic	cone_quadratic_coeffs(const t_cone *cone, t_ray ray)
{
	t_vec3				oc;
	double				dv;
	double				ocv;
	t_quadratic			q;
	t_cone_constants	constants;

	oc = vec3_sub(ray.origin, cone->vertex);
	constants = get_cone_constants(cone);
	dv = vec3_dot(ray.direction, cone->axis);
	ocv = vec3_dot(oc, cone->axis);
	q.a = dv * dv - constants.cos_angle_sq;
	q.b = 2.0 * (dv * ocv - vec3_dot(ray.direction, oc)
			* constants.cos_angle_sq);
	q.c = ocv * ocv - vec3_dot(oc, oc) * constants.cos_angle_sq;
	return (q);
}


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
			hit->color = plane->color;
			hit->obj_type = PLANE;
			hit->hit_side = -1;
			return (1);
		}
	}
	return (0);
}
