#ifndef SCENE_H
# define SCENE_H

# include "math_utils_bonus.h"

/* Ray type */
typedef struct s_ray
{
	t_vec3			origin;
	t_vec3			direction;
}					t_ray;

/* Scene/object types */
# define SPHERE 1
# define PLANE 2
# define CYLINDER 3
# define CONE 4
# define MAX_OBJECTS 100

typedef struct s_camera
{
	t_point3		position;
	t_vec3			orientation;
	double			fov;
}					t_camera;

typedef struct s_camera_vectors
{
	t_vec3			forward;
	t_vec3			right;
	t_vec3			up;
}					t_camera_vectors;

typedef struct s_ambient
{
	double			ratio;
	t_color3		color;
}					t_ambient;

typedef struct s_light
{
	t_point3		position;
	double			brightness;
	t_color3		color;
}					t_light;

typedef struct s_sphere
{
	t_point3		center;
	double			diameter;
	t_color3		color;
}					t_sphere;

typedef struct s_plane
{
	t_point3		point;
	t_vec3			normal;
	t_color3		color;
}					t_plane;

typedef struct s_cylinder
{
	t_point3		center;
	t_vec3			axis;
	double			diameter;
	double			height;
	t_color3		color;
}					t_cylinder;

typedef struct s_cone
{
	t_vec3			center;
	t_vec3			axis;
	double			diameter;
	double			height;
	t_color3		color;
}					t_cone;

typedef struct s_object
{
	int				type;
	union			u_object_data
	{
		t_sphere	sphere;
		t_plane		plane;
		t_cylinder	cylinder;
		t_cone		cone;
	} data;
}					t_object;

typedef struct s_scene
{
	t_camera		camera;
	t_ambient		ambient;
	t_light			light;
	t_object		objects[MAX_OBJECTS];
	int				num_objects;
	int				has_ambient;
	int				has_light;
	int				selected_obj;
}					t_scene;

/* Hit information structure */
typedef struct s_hit
{
	double			t;
	t_vec3			point;
	t_vec3			normal;
	t_color3		color;
	int				obj_type;
	int				obj_index;
	int				hit_side;
}					t_hit;

/* Intersection functions */
int					intersect_sphere(const t_sphere *sphere, t_ray ray,
						t_hit *hit);
int					intersect_plane(const t_plane *plane, t_ray ray,
						t_hit *hit);
int					intersect_cylinder(const t_cylinder *cylinder, t_ray ray,
						t_hit *hit);
int					trace_objects(const t_scene *scene, t_ray ray,
						t_hit *closest_hit);
t_quadratic			sphere_quadratic_coeffs(const t_sphere *sphere, t_ray ray);
t_quadratic			cylinder_quadratic_coeffs(const t_cylinder *cylinder,
						t_ray ray);
t_vec3				cylinder_surface_normal(const t_cylinder *cylinder,
						t_point3 point);

/* Ray tracing functions */
t_ray				generate_camera_ray(const t_scene *scene, int x, int y);
int					trace_ray(const t_scene *scene, t_ray ray);

#endif
