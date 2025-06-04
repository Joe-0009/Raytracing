#include "../../includes/main.h"
#include "../../includes/events.h"
#include "../../includes/transforms.h"
#include <stdio.h>

extern int	g_selected_obj;

void	handle_object_transforms(int keycode, t_scene *scene)
{
	if ((keycode == KEY_P || keycode == KEY_P_MAC)
		&& g_selected_obj < scene->num_objects - 1)
		g_selected_obj++;
	else if ((keycode == KEY_O || keycode == KEY_O_MAC) && g_selected_obj > 0)
		g_selected_obj--;
	else if (keycode == KEY_LEFT || keycode == KEY_LEFT_MAC)
		scene_translate_object(scene, g_selected_obj, vec3_create(-0.3, 0, 0));
	else if (keycode == KEY_RIGHT || keycode == KEY_RIGHT_MAC)
		scene_translate_object(scene, g_selected_obj, vec3_create(0.3, 0, 0));
	else if (keycode == KEY_UP || keycode == KEY_UP_MAC)
		scene_translate_object(scene, g_selected_obj, vec3_create(0, 0.3, 0));
	else if (keycode == KEY_DOWN || keycode == KEY_DOWN_MAC)
		scene_translate_object(scene, g_selected_obj, vec3_create(0, -0.3, 0));
	else if (keycode == KEY_PLUS || keycode == KEY_PLUS_MAC)
		scene_scale_object(scene, g_selected_obj, 1.1);
	else if (keycode == KEY_MINUS || keycode == KEY_MINUS_MAC)
		scene_scale_object(scene, g_selected_obj, 0.9);
	else if (keycode == KEY_R || keycode == KEY_R_MAC)
		scene_rotate_object(scene, g_selected_obj, vec3_create(0.2, 0, 0));
	else if (keycode == KEY_T || keycode == KEY_T_MAC)
		scene_rotate_object(scene, g_selected_obj, vec3_create(0, 0.2, 0));
	else if (keycode == KEY_F || keycode == KEY_F_MAC)
		scene_rotate_object(scene, g_selected_obj, vec3_create(-0.2, 0, 0));
	else if (keycode == KEY_G || keycode == KEY_G_MAC)
		scene_rotate_object(scene, g_selected_obj, vec3_create(0, -0.2, 0));
}
