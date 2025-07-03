#include "../includes/events_bonus.h"
#include "../includes/scene_bonus.h"
#include <stdio.h>

/*
** Transform a camera
*/
void	transform_camera(t_camera *camera, t_transform *transform)
{
	camera->position = matrix4_transform_point(transform->matrix,
			camera->position);
	camera->orientation = matrix4_transform_direction(transform->matrix,
			camera->orientation);
}

/*
** Translate camera in scene
*/
void	scene_translate_camera(t_scene *scene, t_vec3 delta)
{
	t_transform	transform;

	transform = transform_identity();
	transform_translate(&transform, delta);
	transform_camera(&scene->camera, &transform);
}

/*
** Rotate camera in scene
*/
void	scene_rotate_camera(t_scene *scene, t_vec3 rotation)
{
	t_transform	transform;

	transform = transform_identity();
	transform_rotate(&transform, rotation);
	transform_camera(&scene->camera, &transform);
}
