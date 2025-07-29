#ifndef EVENTS_BONUS_H
# define EVENTS_BONUS_H

# include "minirt_app.h"

/* Key codes for Linux (X11) */
# define KEY_ESC        65307
# define KEY_W          119
# define KEY_A          97
# define KEY_S          115
# define KEY_D          100
# define KEY_Q          113
# define KEY_E          101
# define KEY_UP         65362
# define KEY_DOWN       65364
# define KEY_LEFT       65361
# define KEY_RIGHT      65363
# define KEY_PLUS       61
# define KEY_MINUS      45
# define KEY_R          114
# define KEY_T          116
# define KEY_F          102
# define KEY_G          103
# define KEY_I          105
# define KEY_J          106
# define KEY_K          107
# define KEY_L          108
# define KEY_SPACE      32
# define KEY_P          112
# define KEY_O          111
# define KEY_U          117
# define KEY_Y          121
# define KEY_H          104
# define KEY_N          110
# define KEY_M          109
# define KEY_B          98
# define KEY_V          118
# define KEY_C          99
# define KEY_COMMA      44
# define KEY_DOT        46



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
void	handle_light_movement(int keycode, t_scene *scene);
#endif
