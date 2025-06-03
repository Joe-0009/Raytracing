#ifndef TRANSFORMS_H
# define TRANSFORMS_H

# include "math_utils.h"
# include "scene.h"

/*
** Transformation matrix structure
*/
typedef struct s_matrix4
{
	double	m[4][4];
}	t_matrix4;

/*
** Transform structure for objects
*/
typedef struct s_transform
{
	t_vec3		translation;
	t_vec3		rotation;		/* Rotation angles in radians (x, y, z) */
	t_vec3		scale;			/* Scale factors (x, y, z) */
	t_matrix4	matrix;			/* Combined transformation matrix */
}	t_transform;

/*
** Matrix operations
*/
t_matrix4	matrix4_identity(void);
t_matrix4	matrix4_multiply(t_matrix4 a, t_matrix4 b);
t_matrix4	matrix4_translation(t_vec3 translation);
t_matrix4	matrix4_rotation_x(double angle);
t_matrix4	matrix4_rotation_y(double angle);
t_matrix4	matrix4_rotation_z(double angle);
t_matrix4	matrix4_scale(t_vec3 scale);
t_vec3		matrix4_transform_point(t_matrix4 m, t_vec3 point);
t_vec3		matrix4_transform_direction(t_matrix4 m, t_vec3 direction);

/*
** Transform operations
*/
t_transform	transform_identity(void);
void		transform_update_matrix(t_transform *transform);
void		transform_translate(t_transform *transform, t_vec3 translation);
void		transform_rotate(t_transform *transform, t_vec3 rotation);
void		transform_scale_uniform(t_transform *transform, double scale);
void		transform_scale(t_transform *transform, t_vec3 scale);

/*
** Object transformation
*/
void		transform_sphere(t_sphere *sphere, t_transform *transform);
void		transform_plane(t_plane *plane, t_transform *transform);
void		transform_cylinder(t_cylinder *cylinder, t_transform *transform);
void		transform_cone(t_cone *cone, t_transform *transform);
void		transform_camera(t_camera *camera, t_transform *transform);

/*
** Scene transformation utilities
*/
void		scene_translate_object(t_scene *scene, int obj_index, t_vec3 delta);
void		scene_rotate_object(t_scene *scene, int obj_index, t_vec3 rotation);
void		scene_scale_object(t_scene *scene, int obj_index, double scale);
void		scene_translate_camera(t_scene *scene, t_vec3 delta);
void		scene_rotate_camera(t_scene *scene, t_vec3 rotation);

#endif
