#include "../includes/events_bonus.h"
#include "../includes/scene_bonus.h"
#include <stdio.h>

/*
** Create a unified transform for translation, rotation, and scaling
*/
t_transform	create_unified_transform(t_vec3 translation, t_vec3 rotation, t_vec3 scale)
{
	t_transform	transform;

	transform = transform_identity();
	transform.translation = translation;
	transform.rotation = rotation;
	transform.scale = scale;
	transform_update_matrix(&transform);
	return (transform);
}

/*
** Apply a unified transformation to any object type
*/
void	apply_transform_to_object(t_object *object, t_transform *transform)
{
	if (object->type == SPHERE)
		transform_sphere(&object->data.sphere, transform);
	else if (object->type == PLANE)
		transform_plane(&object->data.plane, transform);
	else if (object->type == CYLINDER)
		transform_cylinder(&object->data.cylinder, transform);
	else if (object->type == CONE)
		transform_cone(&object->data.cone, transform);
}

/*
** Create rotation-only transform using Euler angles
*/
t_transform	create_rotation_transform(t_vec3 rotation)
{
	t_transform	transform;

	transform = transform_identity();
	transform.rotation = rotation;
	transform_update_matrix(&transform);
	return (transform);
}

/*
** Create scale-only transform
*/
t_transform	create_scale_transform(t_vec3 scale)
{
	t_transform	transform;

	transform = transform_identity();
	transform.scale = scale;
	transform_update_matrix(&transform);
	return (transform);
}

/*
** Create translation-only transform
*/
t_transform	create_translation_transform(t_vec3 translation)
{
	t_transform	transform;

	transform = transform_identity();
	transform.translation = translation;
	transform_update_matrix(&transform);
	return (transform);
}

/*
** Compose multiple transforms into a single transform
*/
t_transform	compose_transforms(t_transform *transforms, int count)
{
	t_transform	result;
	int			i;

	result = transform_identity();
	i = 0;
	while (i < count)
	{
		result.matrix = matrix4_multiply(result.matrix, transforms[i].matrix);
		i++;
	}
	return (result);
}

/*
** High-level scene transformation function
*/
void	scene_transform_object_unified(t_scene *scene, int obj_index,
		t_vec3 translation, t_vec3 rotation, t_vec3 scale)
{
	t_transform	transform;

	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;

	transform = create_unified_transform(translation, rotation, scale);
	apply_transform_to_object(&scene->objects[obj_index], &transform);
}
