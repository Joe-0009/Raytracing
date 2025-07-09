#include "../../includes/events_bonus.h"
#include "../../includes/minirt_app_bonus.h"
#include "../../includes/scene_bonus.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

void	handle_object_scale_rotation(int keycode, t_scene *scene)
{
	if (keycode == KEY_PLUS)
		scene_scale_object(scene, scene->selected_obj, 1.1);
	else if (keycode == KEY_MINUS)
		scene_scale_object(scene, scene->selected_obj, 0.9);
	else if (keycode == KEY_R || keycode == KEY_T || keycode == KEY_F
		|| keycode == KEY_G)
	{
		if (keycode == KEY_R)
			scene_rotate_object(scene, scene->selected_obj, vec3_create(0.2, 0,
					0));
		else if (keycode == KEY_T)
			scene_rotate_object(scene, scene->selected_obj, vec3_create(0, 0.2,
					0));
		else if (keycode == KEY_F)
			scene_rotate_object(scene, scene->selected_obj, vec3_create(-0.2, 0,
					0));
		else if (keycode == KEY_G)
			scene_rotate_object(scene, scene->selected_obj, vec3_create(0, -0.2,
					0));
	}
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
}

void	handle_sphere_texture_rotation(int keycode, t_scene *scene)
{
	t_sphere	*sphere;
	double		rotation_step;

	if (scene->selected_obj >= scene->num_objects 
		|| scene->objects[scene->selected_obj].type != SPHERE)
		return ;
	
	sphere = &scene->objects[scene->selected_obj].data.sphere;
	rotation_step = M_PI / 16.0; // 11.25 degrees per step
	
	if (keycode == KEY_U)
	{
		// Rotate texture clockwise
		sphere->texture.rotation_z += rotation_step;
		if (sphere->texture.rotation_z >= 2.0 * M_PI)
			sphere->texture.rotation_z -= 2.0 * M_PI;
	}
	else if (keycode == KEY_Y)
	{
		// Rotate texture counter-clockwise
		sphere->texture.rotation_z -= rotation_step;
		if (sphere->texture.rotation_z < 0.0)
			sphere->texture.rotation_z += 2.0 * M_PI;
	}
	else if (keycode == KEY_H)
	{
		// Rotate bump map clockwise
		sphere->bump.rotation_z += rotation_step;
		if (sphere->bump.rotation_z >= 2.0 * M_PI)
			sphere->bump.rotation_z -= 2.0 * M_PI;
	}
	else if (keycode == KEY_N)
	{
		// Rotate bump map counter-clockwise
		sphere->bump.rotation_z -= rotation_step;
		if (sphere->bump.rotation_z < 0.0)
			sphere->bump.rotation_z += 2.0 * M_PI;
	}
}
