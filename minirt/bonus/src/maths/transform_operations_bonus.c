// #include "../includes/events_bonus.h"
// #include "../includes/scene_bonus.h"
// #include <stdio.h>
// #include <math.h>

// /*
// ** Create identity transform
// */
// t_transform	transform_identity(void)
// {
// 	t_transform	transform;

// 	transform.translation = vec3_create(0, 0, 0);
// 	transform.rotation = vec3_create(0, 0, 0);
// 	transform.scale = vec3_create(1, 1, 1);
// 	transform.matrix = matrix4_identity();
// 	return (transform);
// }

// /*
// ** Add translation to transform
// */
// void	transform_translate(t_transform *transform, t_vec3 translation)
// {
// 	transform->translation = vec3_add(transform->translation, translation);
// 	transform_update_matrix(transform);
// }

// /*
// ** Add rotation to transform
// */
// void	transform_rotate(t_transform *transform, t_vec3 rotation)
// {
// 	transform->rotation = vec3_add(transform->rotation, rotation);
// 	transform_update_matrix(transform);
// }

// /*
// ** Set uniform scale for transform
// */
// void	transform_scale_uniform(t_transform *transform, double scale)
// {
// 	transform->scale = vec3_mult(vec3_create(1, 1, 1), scale);
// 	transform_update_matrix(transform);
// }

// /*
// ** Update transformation matrix from translation, rotation, scale
// ** Order: Scale -> Rotate (Z,Y,X) -> Translate
// */
// void	transform_update_matrix(t_transform *transform)
// {
// 	t_matrix4	scale_matrix;
// 	t_matrix4	rotation_x;
// 	t_matrix4	rotation_y;
// 	t_matrix4	rotation_z;
// 	t_matrix4	rotation_combined;
// 	t_matrix4	translation_matrix;

// 	scale_matrix = matrix4_scale(transform->scale);
// 	rotation_x = matrix4_rotation_x(transform->rotation.x);
// 	rotation_y = matrix4_rotation_y(transform->rotation.y);
// 	rotation_z = matrix4_rotation_z(transform->rotation.z);
// 	translation_matrix = matrix4_translation(transform->translation);

// 	// Combine rotations: Z * Y * X
// 	rotation_combined = matrix4_multiply(rotation_z, rotation_y);
// 	rotation_combined = matrix4_multiply(rotation_combined, rotation_x);
	
// 	// Final matrix: T * R * S
// 	transform->matrix = matrix4_multiply(rotation_combined, scale_matrix);
// 	transform->matrix = matrix4_multiply(translation_matrix, transform->matrix);
// }

// /*
// ** Transform object using unified system
// */
// static void	transform_object_unified(t_object *object, t_transform *transform)
// {
// 	if (object->type == SPHERE)
// 		transform_sphere(&object->data.sphere, transform);
// 	else if (object->type == PLANE)
// 		transform_plane(&object->data.plane, transform);
// 	else if (object->type == CYLINDER)
// 		transform_cylinder(&object->data.cylinder, transform);
// 	else if (object->type == CONE)
// 		transform_cone(&object->data.cone, transform);
// }

// /*
// ** Translate object in scene
// */
// void	scene_translate_object(t_scene *scene, int obj_index, t_vec3 delta)
// {
// 	t_transform	transform;

// 	if (obj_index < 0 || obj_index >= scene->num_objects)
// 		return ;
	
// 	transform = transform_identity();
// 	transform_translate(&transform, delta);
// 	transform_object_unified(&scene->objects[obj_index], &transform);
// }

// /*
// ** Rotate object in scene using unified matrix system
// */
// void	scene_rotate_object(t_scene *scene, int obj_index, t_vec3 rotation)
// {
// 	t_transform	transform;

// 	if (obj_index < 0 || obj_index >= scene->num_objects)
// 		return ;
	
// 	transform = transform_identity();
// 	transform_rotate(&transform, rotation);
// 	transform_object_unified(&scene->objects[obj_index], &transform);
// }

// /*
// ** Scale object in scene
// */
// void	scene_scale_object(t_scene *scene, int obj_index, double scale)
// {
// 	t_transform	transform;

// 	if (obj_index < 0 || obj_index >= scene->num_objects)
// 		return ;
	
// 	transform = transform_identity();
// 	transform_scale_uniform(&transform, scale);
// 	transform_object_unified(&scene->objects[obj_index], &transform);
// }

// /*
// ** Apply combined transformation to object
// */
// void	scene_transform_object(t_scene *scene, int obj_index, 
// 		t_vec3 translation, t_vec3 rotation, double scale)
// {
// 	t_transform	transform;

// 	if (obj_index < 0 || obj_index >= scene->num_objects)
// 		return ;
	
// 	transform = transform_identity();
// 	transform_translate(&transform, translation);
// 	transform_rotate(&transform, rotation);
// 	transform_scale_uniform(&transform, scale);
// 	transform_object_unified(&scene->objects[obj_index], &transform);
// }

// /*
// ** Direct object translation without creating transform object
// ** More efficient for simple translations
// */
// void	scene_translate_object_direct(t_scene *scene, int obj_index, t_vec3 delta)
// {
// 	t_matrix4	translation_matrix;

// 	if (obj_index < 0 || obj_index >= scene->num_objects)
// 		return ;
	
// 	translation_matrix = matrix4_translation(delta);
	
// 	if (scene->objects[obj_index].type == SPHERE)
// 	{
// 		scene->objects[obj_index].data.sphere.center = 
// 			matrix4_transform_point(translation_matrix,
// 				scene->objects[obj_index].data.sphere.center);
// 	}
// 	else if (scene->objects[obj_index].type == PLANE)
// 	{
// 		scene->objects[obj_index].data.plane.point = 
// 			matrix4_transform_point(translation_matrix,
// 				scene->objects[obj_index].data.plane.point);
// 	}
// 	else if (scene->objects[obj_index].type == CYLINDER)
// 	{
// 		scene->objects[obj_index].data.cylinder.center = 
// 			matrix4_transform_point(translation_matrix,
// 				scene->objects[obj_index].data.cylinder.center);
// 	}
// 	else if (scene->objects[obj_index].type == CONE)
// 	{
// 		scene->objects[obj_index].data.cone.vertex = 
// 			matrix4_transform_point(translation_matrix,
// 				scene->objects[obj_index].data.cone.vertex);
// 	}
// }
