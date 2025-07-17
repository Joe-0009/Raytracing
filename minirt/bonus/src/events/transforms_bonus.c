#include "../../includes/events_bonus.h"
#include "../../includes/scene_bonus.h"
#include <stdio.h>
#include <math.h>

/*
** Translate object directly using matrix operations
*/
void	scene_translate_object(t_scene *scene, int obj_index, t_vec3 delta)
{
	t_matrix4	translation_matrix;

	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;
	
	translation_matrix = matrix4_translation(delta);
	
	if (scene->objects[obj_index].type == SPHERE)
	{
		scene->objects[obj_index].data.sphere.center = 
			matrix4_transform_point(translation_matrix,
				scene->objects[obj_index].data.sphere.center);
	}
	else if (scene->objects[obj_index].type == PLANE)
	{
		scene->objects[obj_index].data.plane.point = 
			matrix4_transform_point(translation_matrix,
				scene->objects[obj_index].data.plane.point);
	}
	else if (scene->objects[obj_index].type == CYLINDER)
	{
		scene->objects[obj_index].data.cylinder.center = 
			matrix4_transform_point(translation_matrix,
				scene->objects[obj_index].data.cylinder.center);
	}
	else if (scene->objects[obj_index].type == CONE)
	{
		scene->objects[obj_index].data.cone.vertex = 
			matrix4_transform_point(translation_matrix,
				scene->objects[obj_index].data.cone.vertex);
	}
}

/*
** Rotate object directly using matrix operations
*/
void	scene_rotate_object(t_scene *scene, int obj_index, t_vec3 rotation)
{
	t_matrix4	rotation_x;
	t_matrix4	rotation_y;
	t_matrix4	rotation_z;
	t_matrix4	combined_rotation;

	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;
	
	// Create individual rotation matrices
	rotation_x = matrix4_rotation_x(rotation.x);
	rotation_y = matrix4_rotation_y(rotation.y);
	rotation_z = matrix4_rotation_z(rotation.z);

	// Combine rotations: Z * Y * X
	combined_rotation = matrix4_multiply(rotation_z, rotation_y);
	combined_rotation = matrix4_multiply(combined_rotation, rotation_x);

	// Apply rotation based on object type
	if (scene->objects[obj_index].type == SPHERE)
	{
		// For spheres, we can rotate around center (affects texture/normal mapping)
		// Position stays the same, only orientation changes
		// Note: This assumes spheres have orientation data for texture mapping
	}
	else if (scene->objects[obj_index].type == PLANE)
	{
		// Rotate plane normal
		scene->objects[obj_index].data.plane.normal = 
			matrix4_transform_direction(combined_rotation,
				scene->objects[obj_index].data.plane.normal);
		scene->objects[obj_index].data.plane.normal = 
			vec3_normalize(scene->objects[obj_index].data.plane.normal);
	}
	else if (scene->objects[obj_index].type == CYLINDER)
	{
		// Rotate cylinder axis
		scene->objects[obj_index].data.cylinder.axis = 
			matrix4_transform_direction(combined_rotation,
				scene->objects[obj_index].data.cylinder.axis);
		scene->objects[obj_index].data.cylinder.axis = 
			vec3_normalize(scene->objects[obj_index].data.cylinder.axis);
	}
	else if (scene->objects[obj_index].type == CONE)
	{
		// Rotate cone axis
		scene->objects[obj_index].data.cone.axis = 
			matrix4_transform_direction(combined_rotation,
				scene->objects[obj_index].data.cone.axis);
		scene->objects[obj_index].data.cone.axis = 
			vec3_normalize(scene->objects[obj_index].data.cone.axis);
	}
}

/*
** Scale object directly using matrix operations
*/
void	scene_scale_object(t_scene *scene, int obj_index, double scale)
{
	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;
	
	if (scene->objects[obj_index].type == SPHERE)
	{
		scene->objects[obj_index].data.sphere.radius *= scale;
	}
	else if (scene->objects[obj_index].type == CYLINDER)
	{
		scene->objects[obj_index].data.cylinder.diameter *= scale;
		scene->objects[obj_index].data.cylinder.diameter *= scale;
	}
	else if (scene->objects[obj_index].type == CONE)
	{
		scene->objects[obj_index].data.cone.height *= scale;
		scene->objects[obj_index].data.cone.height *= scale;
	}
	// Note: Planes don't scale as they're infinite
}

/*
** Rotate object around specific axis
*/
// void	scene_rotate_object_axis(t_scene *scene, int obj_index, t_vec3 axis, double angle)
// {
// 	t_matrix4	rotation_matrix;

// 	if (obj_index < 0 || obj_index >= scene->num_objects)
// 		return ;
	
// 	rotation_matrix = matrix4_rotation_axis(vec3_normalize(axis), angle);

// 	// Apply rotation based on object type
// 	if (scene->objects[obj_index].type == PLANE)
// 	{
// 		scene->objects[obj_index].data.plane.normal = 
// 			matrix4_transform_direction(rotation_matrix,
// 				scene->objects[obj_index].data.plane.normal);
// 		scene->objects[obj_index].data.plane.normal = 
// 			vec3_normalize(scene->objects[obj_index].data.plane.normal);
// 	}
// 	else if (scene->objects[obj_index].type == CYLINDER)
// 	{
// 		scene->objects[obj_index].data.cylinder.axis = 
// 			matrix4_transform_direction(rotation_matrix,
// 				scene->objects[obj_index].data.cylinder.axis);
// 		scene->objects[obj_index].data.cylinder.axis = 
// 			vec3_normalize(scene->objects[obj_index].data.cylinder.axis);
// 	}
// 	else if (scene->objects[obj_index].type == CONE)
// 	{
// 		scene->objects[obj_index].data.cone.axis = 
// 			matrix4_transform_direction(rotation_matrix,
// 				scene->objects[obj_index].data.cone.axis);
// 		scene->objects[obj_index].data.cone.axis = 
// 			vec3_normalize(scene->objects[obj_index].data.cone.axis);
// 	}
// }

/*
** Rotate object around its center (for positioned objects)
*/
// void	scene_rotate_object_around_center(t_scene *scene, int obj_index, t_vec3 rotation)
// {
// 	t_vec3		center;
// 	t_matrix4	rotation_x;
// 	t_matrix4	rotation_y;
// 	t_matrix4	rotation_z;
// 	t_matrix4	combined_rotation;
// 	t_matrix4	to_origin;
// 	t_matrix4	from_origin;
// 	t_matrix4	final_transform;

// 	if (obj_index < 0 || obj_index >= scene->num_objects)
// 		return ;
	
// 	// Get object center
// 	if (scene->objects[obj_index].type == SPHERE)
// 		center = scene->objects[obj_index].data.sphere.center;
// 	else if (scene->objects[obj_index].type == CYLINDER)
// 		center = scene->objects[obj_index].data.cylinder.center;
// 	else if (scene->objects[obj_index].type == CONE)
// 		center = scene->objects[obj_index].data.cone.center;
// 	else
// 		return ; // Planes don't have a center

// 	// Create rotation matrices
// 	rotation_x = matrix4_rotation_x(rotation.x);
// 	rotation_y = matrix4_rotation_y(rotation.y);
// 	rotation_z = matrix4_rotation_z(rotation.z);
// 	combined_rotation = matrix4_multiply(rotation_z, rotation_y);
// 	combined_rotation = matrix4_multiply(combined_rotation, rotation_x);

// 	// Create translation matrices
// 	to_origin = matrix4_translation(vec3_mult(center, -1));
// 	from_origin = matrix4_translation(center);

// 	// Final transform: translate to origin, rotate, translate back
// 	final_transform = matrix4_multiply(combined_rotation, to_origin);
// 	final_transform = matrix4_multiply(from_origin, final_transform);

// 	// Apply transformation (this would need the individual transform functions)
// 	// For now, just rotate the axes/normals
// 	scene_rotate_object(scene, obj_index, rotation);
// }

/*
** Combined object transformation (translation + rotation + scale)
*/
void	scene_transform_object(t_scene *scene, int obj_index, 
		t_vec3 translation, t_vec3 rotation, double scale)
{
	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;
	
	// Apply transformations in order: scale, rotate, translate
	scene_scale_object(scene, obj_index, scale);
	scene_rotate_object(scene, obj_index, rotation);
	scene_translate_object(scene, obj_index, translation);
}
