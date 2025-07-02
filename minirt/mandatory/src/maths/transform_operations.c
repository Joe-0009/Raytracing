#include "../includes/events.h"
#include "../includes/scene.h"
#include <stdio.h>

/*
** Add translation to transform
*/
void	transform_translate(t_transform *transform, t_vec3 translation)
{
	transform->translation = vec3_add(transform->translation, translation);
	transform_update_matrix(transform);
}

/*
** Add rotation to transform
*/
void	transform_rotate(t_transform *transform, t_vec3 rotation)
{
	transform->rotation = vec3_add(transform->rotation, rotation);
	transform_update_matrix(transform);
}

/*
** Apply uniform scale to transform
*/
void	transform_scale_uniform(t_transform *transform, double scale)
{
	transform->scale = vec3_mult(transform->scale, scale);
	transform_update_matrix(transform);
}

/*
** Apply non-uniform scale to transform
*/
void	transform_scale(t_transform *transform, t_vec3 scale)
{
	transform->scale.x *= scale.x;
	transform->scale.y *= scale.y;
	transform->scale.z *= scale.z;
	transform_update_matrix(transform);
}