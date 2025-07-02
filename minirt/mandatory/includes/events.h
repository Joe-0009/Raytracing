#ifndef EVENTS_H
# define EVENTS_H

# include "minirt_app.h"

/* Key codes */
# define KEY_ESC 65307
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_Q 113
# define KEY_E 101
# define KEY_UP 65362
# define KEY_DOWN 65364
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define KEY_PLUS 65451
# define KEY_MINUS 65453
# define KEY_R 114
# define KEY_T 116
# define KEY_F 102
# define KEY_G 103
# define KEY_I 105
# define KEY_J 106
# define KEY_K 107
# define KEY_L 108
# define KEY_SPACE 32
# define KEY_P 112
# define KEY_O 111

/* Transform type for interactive transformations */
typedef struct s_transform
{
	t_vec3		translation;
	t_vec3		rotation;
	t_vec3		scale;
	t_matrix4	matrix;
}				t_transform;

/* Transform operations */
t_transform		transform_identity(void);
void			transform_update_matrix(t_transform *transform);
void			transform_translate(t_transform *transform, t_vec3 translation);
void			transform_rotate(t_transform *transform, t_vec3 rotation);
void			transform_scale_uniform(t_transform *transform, double scale);
void			transform_scale(t_transform *transform, t_vec3 scale);

/* Object transformation */
void			transform_sphere(t_sphere *sphere, t_transform *transform);
void			transform_plane(t_plane *plane, t_transform *transform);
void			transform_cylinder(t_cylinder *cylinder,
					t_transform *transform);
void			transform_camera(t_camera *camera, t_transform *transform);

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
