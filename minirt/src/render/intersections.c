#include "../includes/minirt_app.h"
#include "../includes/scene_math.h"

/*
** Check intersection with sphere object
** Returns 1 if hit, 0 if no hit
*/
static int	trace_sphere(const t_object *obj, t_ray ray, t_hit *closest_hit,
		int index)
{
	if (intersect_sphere(&obj->data.sphere, ray, closest_hit))
	{
		closest_hit->obj_index = index;
		return (1);
	}
	return (0);
}

/*
** Check intersection with plane object
** Returns 1 if hit, 0 if no hit
*/
static int	trace_plane(const t_object *obj, t_ray ray, t_hit *closest_hit,
		int index)
{
	if (intersect_plane(&obj->data.plane, ray, closest_hit))
	{
		closest_hit->obj_index = index;
		return (1);
	}
	return (0);
}

/*
** Check intersection with cylinder object
** Returns 1 if hit, 0 if no hit
*/
static int	trace_cylinder(const t_object *obj, t_ray ray, t_hit *closest_hit,
		int index)
{
	if (intersect_cylinder(&obj->data.cylinder, ray, closest_hit))
	{
		closest_hit->obj_index = index;
		return (1);
	}
	return (0);
}

/*
** Check intersection with cone object
** Returns 1 if hit, 0 if no hit
*/
static int	trace_cone(const t_object *obj, t_ray ray, t_hit *closest_hit,
		int index)
{
	if (intersect_cone(&obj->data.cone, ray, closest_hit))
	{
		closest_hit->obj_index = index;
		return (1);
	}
	return (0);
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
		if (scene->objects[i].type == SPHERE && trace_sphere(&scene->objects[i],
				ray, closest_hit, i))
			hit_found = 1;
		else if (scene->objects[i].type == PLANE
			&& trace_plane(&scene->objects[i], ray, closest_hit, i))
			hit_found = 1;
		else if (scene->objects[i].type == CYLINDER
			&& trace_cylinder(&scene->objects[i], ray, closest_hit, i))
			hit_found = 1;
		else if (scene->objects[i].type == CONE
			&& trace_cone(&scene->objects[i], ray, closest_hit, i))
			hit_found = 1;
		i++;
	}
	return (hit_found);
}
