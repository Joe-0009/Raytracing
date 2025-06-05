#include "../includes/minirt_app.h"
#include "../includes/scene_math.h"
#include <math.h>

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
