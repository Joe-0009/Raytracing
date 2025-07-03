#ifndef SCENE_MATH_H
# define SCENE_MATH_H

# include <math.h>

// --- Math/vector types ---
typedef struct s_vec3
{
	double			x;
	double			y;
	double			z;
}					t_vec3;

typedef struct s_quadratic
{
	double			a;
	double			b;
	double			c;
}					t_quadratic;

typedef t_vec3		t_point3;
typedef t_vec3		t_color3;

// --- Scene/object types ---
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

typedef struct s_material
{
	t_color3		color;
}					t_material;

typedef struct s_sphere
{
	t_point3		center;
	double			diameter;
	t_color3		color;
	t_material		material;
}					t_sphere;

typedef struct s_plane
{
	t_point3		point;
	t_vec3			normal;
	t_color3		color;
	t_material		material;
}					t_plane;

typedef struct s_cylinder
{
	t_point3		center;
	t_vec3			axis;
	double			diameter;
	double			height;
	t_color3		color;
	t_material		material;
}					t_cylinder;

typedef struct s_cone
{
	t_point3		vertex;
	t_vec3			axis;
	double			angle;
	double			height;
	t_color3		color;
	t_material		material;
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
}					t_scene;

// --- Matrix and transform types ---
typedef struct s_matrix4
{
	double			m[4][4];
}					t_matrix4;

typedef struct s_transform
{
	t_vec3			translation;
	t_vec3			rotation;
	t_vec3			scale;
	t_matrix4		matrix;
}					t_transform;

// --- Ray type ---
typedef struct s_ray
{
	t_vec3			origin;
	t_vec3			direction;
}					t_ray;

// --- Math/vector utilities ---
t_vec3				vec3_create(double x, double y, double z);
t_vec3				vec3_add(t_vec3 v1, t_vec3 v2);
t_vec3				vec3_sub(t_vec3 v1, t_vec3 v2);
t_vec3				vec3_mult(t_vec3 v, double t);
t_vec3				vec3_div(t_vec3 v, double t);
t_vec3				vec3_cross(t_vec3 v1, t_vec3 v2);
t_vec3				vec3_normalize(t_vec3 v);
double				vec3_dot(t_vec3 v1, t_vec3 v2);
double				vec3_length(t_vec3 v);
double				vec3_length_squared(t_vec3 v);
t_vec3				reflect(t_vec3 v, t_vec3 n);
t_vec3				vec3_rotate_around_axis(t_vec3 v, t_vec3 axis,
						double angle);
double				solve_quadratic(double a, double b, double c, double min_t);

// --- Matrix operations ---
t_matrix4			matrix4_identity(void);
t_matrix4			matrix4_multiply(t_matrix4 a, t_matrix4 b);
t_matrix4			matrix4_translation(t_vec3 translation);
t_matrix4			matrix4_rotation_x(double angle);
t_matrix4			matrix4_rotation_y(double angle);
t_matrix4			matrix4_rotation_z(double angle);
t_matrix4			matrix4_scale(t_vec3 scale);
t_vec3				matrix4_transform_point(t_matrix4 m, t_vec3 point);
t_vec3				matrix4_transform_direction(t_matrix4 m, t_vec3 direction);

// --- Transform operations ---
t_transform			transform_identity(void);
void				transform_update_matrix(t_transform *transform);
void				transform_translate(t_transform *transform,
						t_vec3 translation);
void				transform_rotate(t_transform *transform, t_vec3 rotation);
void				transform_scale_uniform(t_transform *transform,
						double scale);
void				transform_scale(t_transform *transform, t_vec3 scale);

// --- Object transformation ---
void				transform_sphere(t_sphere *sphere, t_transform *transform);
void				transform_plane(t_plane *plane, t_transform *transform);
void				transform_cylinder(t_cylinder *cylinder,
						t_transform *transform);
void				transform_cone(t_cone *cone, t_transform *transform);
void				transform_camera(t_camera *camera, t_transform *transform);

// --- Scene transformation utilities ---
void				scene_translate_object(t_scene *scene, int obj_index,
						t_vec3 delta);
void				scene_rotate_object(t_scene *scene, int obj_index,
						t_vec3 rotation);
void				scene_scale_object(t_scene *scene, int obj_index,
						double scale);
void				scene_translate_camera(t_scene *scene, t_vec3 delta);
void				scene_rotate_camera(t_scene *scene, t_vec3 rotation);

// --- Ray tracing functions ---
t_ray				generate_camera_ray(const t_scene *scene, int x, int y);
int					trace_ray(const t_scene *scene, t_ray ray);

#endif
