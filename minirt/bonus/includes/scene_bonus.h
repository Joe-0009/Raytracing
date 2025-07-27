#ifndef SCENE_BONUS_H
# define SCENE_BONUS_H

# include "math_utils_bonus.h"

/* Unified surface map structure for textures and bump maps */

/* Map type constants */
# define MAP_TYPE_TEXTURE 0
# define MAP_TYPE_BUMP 1

/* UV coordinates for texture mapping */
typedef struct s_uv
{
	double			u;
	double			v;
}					t_uv;

typedef struct s_surface_map
{
	int				width;
	int				height;
	unsigned char	*data;
	int				is_active;
	void			*mlx_img;
	char			*path;
	int				map_type;
	t_uv			rotation_uv;
}					t_surface_map;
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
# define MAX_LIGHTS 10

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
	double			radius;
	t_color3		color;
	t_surface_map	texture;
	t_surface_map	bump;
	int				checkerboard;
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

/*
** Precompute cone constants for optimization
*/
typedef struct s_cone_constants
{
	double			cos_angle;
	double			sin_angle;
	double			cos_angle_sq;
	double			tan_half_angle;
}					t_cone_constants;

typedef struct s_cone
{
	t_point3		vertex;
	t_vec3			axis;
	double			angle;
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
	t_light			light[MAX_LIGHTS];
	int				nbr_of_lights;
	t_object		objects[MAX_OBJECTS];
	int				num_objects;
	int				has_ambient;
	int				selected_obj;
}					t_scene;

/* Hit information structure */
typedef struct s_hit
{
	double			t;
	t_vec3			point;
	t_vec3			normal;
	t_color3		color;
	t_uv			uv;
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
int					intersect_cone(const t_cone *cone, t_ray ray, t_hit *hit);
int					trace_objects(const t_scene *scene, t_ray ray,
						t_hit *closest_hit);
t_quadratic			cylinder_quadratic_coeffs(const t_cylinder *cylinder,
						t_ray ray);
t_cone_constants	get_cone_constants(const t_cone *cone);
t_quadratic			cone_quadratic_coeffs(const t_cone *cone, t_ray ray);

/* Ray tracing functions */
t_ray				generate_camera_ray(const t_scene *scene, int x, int y);
int					trace_ray(const t_scene *scene, t_ray ray);

/* Texture functions */
void				load_surface_map(void *mlx, t_surface_map *surface_map);
t_color3			sample_texture(const t_surface_map *texture, t_uv uv);
double				sample_bump_map(const t_surface_map *bump, t_uv uv);
t_vec3				apply_bump_mapping(t_vec3 normal, t_uv uv,
						const t_surface_map *bump, t_vec3 tangent,
						t_vec3 bitangent);
t_uv				sphere_uv_mapping(t_vec3 normalized);
void				load_scene_texture_bump(void *mlx, t_scene *scene);

/* Texture rotation functions */
void				set_texture_rotation_degrees(t_surface_map *map, double angle_degrees);
void				set_sphere_texture_rotation(t_sphere *sphere, double angle_degrees);

#endif
