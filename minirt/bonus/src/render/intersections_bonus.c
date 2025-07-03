#include "../includes/minirt_app_bonus.h"
#include "../includes/scene_bonus.h"

/*
** Check intersection with any object type
** Returns 1 if hit, 0 if no hit
*/
static int	trace_object(const t_object *obj, t_ray ray, t_hit *closest_hit,
		int index)
{
	int	hit;

	hit = 0;
	if (obj->type == SPHERE)
		hit = intersect_sphere(&obj->data.sphere, ray, closest_hit);
	else if (obj->type == PLANE)
		hit = intersect_plane(&obj->data.plane, ray, closest_hit);
	else if (obj->type == CYLINDER)
		hit = intersect_cylinder(&obj->data.cylinder, ray, closest_hit);
	if (hit)
		closest_hit->obj_index = index;
	return (hit);
}

/*
** Check intersection with all objects in scene
** Returns 1 if any hit, 0 if no hit
** Optimized with early ray termination
*/
int	trace_objects(const t_scene *scene, t_ray ray, t_hit *closest_hit)
{
	int		i;
	int		hit_found;
	t_hit	temp_hit;
	double	closest_distance;

	hit_found = 0;
	closest_distance = INFINITY;
	if (scene->num_objects == 0)
		return (0);
	i = 0;
	while (i < scene->num_objects)
	{
		temp_hit.t = -1.0;
		if (trace_object(&scene->objects[i], ray, &temp_hit, i))
		{
			if (temp_hit.t > 0.001 && temp_hit.t < closest_distance)
			{
				*closest_hit = temp_hit;
				closest_distance = temp_hit.t;
				hit_found = 1;
			}
		}
		i++;
	}
	return (hit_found);
}
