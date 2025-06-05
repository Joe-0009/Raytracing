#ifndef EVENTS_H
# define EVENTS_H

# include "minirt_app.h"

/* Key codes */
# define KEY_ESC 65307
# define KEY_ESC_MAC 53
# define KEY_W 119
# define KEY_W_MAC 13
# define KEY_A 97
# define KEY_A_MAC 0
# define KEY_S 115
# define KEY_S_MAC 1
# define KEY_D 100
# define KEY_D_MAC 2
# define KEY_Q 113
# define KEY_Q_MAC 12
# define KEY_E 101
# define KEY_E_MAC 14
# define KEY_UP 65362
# define KEY_UP_MAC 126
# define KEY_DOWN 65364
# define KEY_DOWN_MAC 125
# define KEY_LEFT 65361
# define KEY_LEFT_MAC 123
# define KEY_RIGHT 65363
# define KEY_RIGHT_MAC 124
# define KEY_PLUS 65451
# define KEY_PLUS_MAC 24
# define KEY_MINUS 65453
# define KEY_MINUS_MAC 27
# define KEY_R 114
# define KEY_R_MAC 15
# define KEY_T 116
# define KEY_T_MAC 17
# define KEY_F 102
# define KEY_F_MAC 3
# define KEY_G 103
# define KEY_G_MAC 5
# define KEY_I 105
# define KEY_I_MAC 34
# define KEY_J 106
# define KEY_J_MAC 38
# define KEY_K 107
# define KEY_K_MAC 40
# define KEY_L 108
# define KEY_L_MAC 37
# define KEY_SPACE 32
# define KEY_SPACE_MAC 49
# define KEY_P 112
# define KEY_P_MAC 35
# define KEY_O 111
# define KEY_O_MAC 31

/* Global variables */
extern t_scene	*g_scene;
extern int		g_selected_obj;

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
