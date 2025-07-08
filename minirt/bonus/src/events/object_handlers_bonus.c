#include "../../includes/events_bonus.h"
#include "../../includes/minirt_app_bonus.h"
#include "../../includes/scene_bonus.h"
#include <stdio.h>

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
