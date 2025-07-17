#include "../includes/events_bonus.h"
#include "../includes/scene_bonus.h"
#include <stdio.h>

/*
** Transform a sphere
*/
void	transform_sphere(t_sphere *sphere, t_transform *transform)
{
	sphere->center = matrix4_transform_point(transform->matrix, sphere->center);
	if (transform->scale.x == transform->scale.y
		&& transform->scale.y == transform->scale.z)
		sphere->diameter *= transform->scale.x;
}

/*
** Transform a plane
*/
void	transform_plane(t_plane *plane, t_transform *transform)
{
	plane->point = matrix4_transform_point(transform->matrix, plane->point);
	plane->normal = matrix4_transform_direction(transform->matrix,
			plane->normal);
}

/*
** Transform a cylinder
*/
void	transform_cylinder(t_cylinder *cylinder, t_transform *transform)
{
	cylinder->center = matrix4_transform_point(transform->matrix,
			cylinder->center);
	cylinder->axis = matrix4_transform_direction(transform->matrix,
			cylinder->axis);
	if (transform->scale.x == transform->scale.y
		&& transform->scale.y == transform->scale.z)
	{
		cylinder->diameter *= transform->scale.x;
		cylinder->height *= transform->scale.y;
	}
}

void	transform_cone(t_cone *cone, t_transform *transform)
{
	cone->vertex = matrix4_transform_point(transform->matrix, cone->vertex);
	cone->axis = matrix4_transform_direction(transform->matrix, cone->axis);
	if (transform->scale.x == transform->scale.y
		&& transform->scale.y == transform->scale.z)
	{
		cone->angle *= transform->scale.x;
		cone->height *= transform->scale.y;
	}
}
