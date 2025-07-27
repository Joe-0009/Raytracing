#ifndef EVENTS_BONUS_H
# define EVENTS_BONUS_H

# include "minirt_app_bonus.h"

/* Key codes for standard MLX */
# define KEY_ESC 53
# define KEY_W 13
# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_Q 12
# define KEY_E 14
# define KEY_UP 126
# define KEY_DOWN 125
# define KEY_LEFT 123
# define KEY_RIGHT 124
# define KEY_PLUS 24
# define KEY_MINUS 27
# define KEY_R 15
# define KEY_T 17
# define KEY_F 3
# define KEY_G 5
# define KEY_I 34
# define KEY_J 38
# define KEY_K 40
# define KEY_L 37
# define KEY_SPACE 49
# define KEY_P 35
# define KEY_O 31
# define KEY_U 32
# define KEY_Y 16
# define KEY_H 4
# define KEY_N 45



/* Scene transformation utilities */
void			scene_translate_object(t_scene *scene, int obj_index,
					t_vec3 delta);
void			scene_rotate_object(t_scene *scene, int obj_index,
					t_vec3 rotation);
void			scene_scale_object(t_scene *scene, int obj_index, double scale);
void			scene_translate_camera(t_scene *scene, t_vec3 delta);
void			scene_rotate_camera(t_scene *scene, t_vec3 rotation);

/* Function prototypes for events */
int				close_window_x(t_vars *vars);
int				close_window_esc(int keycode, t_vars *vars);
int				key_handler(int keycode, t_vars *vars);
void			mlx_hooks(t_vars *vars);
void			draw_new_image(t_vars *vars, t_scene *scene);
void			handle_camera_movement(int keycode, t_scene *scene);
void			handle_camera_rotation(int keycode, t_scene *scene);
void			handle_object_transforms(int keycode, t_scene *scene);
void			print_controls_help(void);

#endif
