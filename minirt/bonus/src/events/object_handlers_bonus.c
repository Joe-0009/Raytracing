#include "../../includes/events_bonus.h"
#include "../../includes/minirt_app_bonus.h"
#include "../../includes/scene_bonus.h"
#include <stdio.h>

void	handle_object_scale_rotation(int keycode, t_scene *scene)
{
	int obj_index;
	
	if (keycode == KEY_PLUS)
		scene_scale_object(scene, scene->selected_obj, 1.1);
	else if (keycode == KEY_MINUS)
		scene_scale_object(scene, scene->selected_obj, 0.9);
	else if (keycode == KEY_R || keycode == KEY_T || keycode == KEY_F || keycode == KEY_G)
	{
		obj_index = scene->selected_obj;
		if (obj_index >= 0 && obj_index < scene->num_objects && scene->objects[obj_index].type == SPHERE)
		{
			// For spheres, rotate the texture instead of the object
			if (keycode == KEY_R)
				scene->objects[obj_index].data.sphere.rotation.x += 0.2;
			else if (keycode == KEY_T)
				scene->objects[obj_index].data.sphere.rotation.y += 0.2;
			else if (keycode == KEY_F)
				scene->objects[obj_index].data.sphere.rotation.x -= 0.2;
			else if (keycode == KEY_G)
				scene->objects[obj_index].data.sphere.rotation.y -= 0.2;
		}
		else
		{
			// For other objects, rotate the object itself
			if (keycode == KEY_R)
				scene_rotate_object(scene, scene->selected_obj, vec3_create(0.2, 0, 0));
			else if (keycode == KEY_T)
				scene_rotate_object(scene, scene->selected_obj, vec3_create(0, 0.2, 0));
			else if (keycode == KEY_F)
				scene_rotate_object(scene, scene->selected_obj, vec3_create(-0.2, 0, 0));
			else if (keycode == KEY_G)
				scene_rotate_object(scene, scene->selected_obj, vec3_create(0, -0.2, 0));
		}
	}
}

void	handle_sphere_texture_rotation(int keycode, t_scene *scene)
{
	int obj_index;
	
	obj_index = scene->selected_obj;
	if (obj_index >= scene->num_objects || scene->objects[obj_index].type != SPHERE)
		return;
		
	if (keycode == KEY_I)  // Rotate texture up (negative X rotation)
		scene->objects[obj_index].data.sphere.rotation.x -= 0.2;
	else if (keycode == KEY_K)  // Rotate texture down (positive X rotation)
		scene->objects[obj_index].data.sphere.rotation.x += 0.2;
	else if (keycode == KEY_J)  // Rotate texture left (negative Y rotation)
		scene->objects[obj_index].data.sphere.rotation.y -= 0.2;
	else if (keycode == KEY_L)  // Rotate texture right (positive Y rotation)
		scene->objects[obj_index].data.sphere.rotation.y += 0.2;
}

void	handle_object_translation(int keycode, t_scene *scene)
{
	if (keycode == KEY_P && scene->selected_obj < scene->num_objects - 1)
		scene->selected_obj++;
	else if (keycode == KEY_O && scene->selected_obj > 0)
		scene->selected_obj--;
	else if (keycode == KEY_LEFT)
		scene_translate_object(scene, scene->selected_obj, vec3_create(-0.3, 0,
				0));
	else if (keycode == KEY_RIGHT)
		scene_translate_object(scene, scene->selected_obj, vec3_create(0.3, 0,
				0));
	else if (keycode == KEY_UP)
		scene_translate_object(scene, scene->selected_obj, vec3_create(0, 0.3,
				0));
	else if (keycode == KEY_DOWN)
		scene_translate_object(scene, scene->selected_obj, vec3_create(0, -0.3,
				0));
}

void	handle_object_transforms(int keycode, t_scene *scene)
{
	handle_object_scale_rotation(keycode, scene);
	handle_object_translation(keycode, scene);
	handle_sphere_texture_rotation(keycode, scene);
}
