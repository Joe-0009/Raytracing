#include "../includes/events.h"
#include "../includes/scene.h"
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
}

/*
** Apply combined transformation to object in scene
*/
void	scene_transform_object(t_scene *scene, int obj_index,
		t_vec3 translation, t_vec3 rotation, t_vec3 scale)
{
	t_transform	transform;

	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;

	transform = create_unified_transform(translation, rotation, scale);
	apply_transform_to_object(&scene->objects[obj_index], &transform);
}

/*
** Create rotation-only transform using Euler angles
*/
t_transform	create_rotation_transform(t_vec3 rotation)
{
	t_matrix4	rotation_x;
	t_matrix4	rotation_y;
	t_matrix4	rotation_z;
	t_matrix4	combined_rotation;
	t_transform	transform;

	// Create individual rotation matrices
	rotation_x = matrix4_rotation_x(rotation.x);
	rotation_y = matrix4_rotation_y(rotation.y);
	rotation_z = matrix4_rotation_z(rotation.z);

	// Combine rotations: Z * Y * X (standard order)
	combined_rotation = matrix4_multiply(rotation_z, rotation_y);
	combined_rotation = matrix4_multiply(combined_rotation, rotation_x);

	// Create transform with rotation matrix
	transform = transform_identity();
	transform.rotation = rotation;
	transform.matrix = combined_rotation;

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
	transform.matrix = matrix4_scale(scale);

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
	transform.matrix = matrix4_translation(translation);

	return (transform);
}

/*
** Matrix-based vector rotation (replaces vec3_rotate_around_axis)
*/
t_vec3	matrix_rotate_vector(t_vec3 vector, t_vec3 axis, double angle)
{
	t_matrix4	rotation_matrix;
	t_vec3		result;

	// Use the advanced Rodrigues' rotation matrix for arbitrary axis
	rotation_matrix = matrix4_rotation_axis(axis, angle);
	result = matrix4_transform_direction(rotation_matrix, vector);

	return (vec3_normalize(result));
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
