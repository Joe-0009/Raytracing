#ifndef SCENE_H
# define SCENE_H

# include "math_utils.h"

# define SPHERE 1
# define PLANE 2
# define CYLINDER 3
# define CONE 4
# define MAX_OBJECTS 100

typedef struct s_camera
{
	t_point3	position;
	t_vec3		orientation;
	double		fov;
} t_camera;

typedef struct s_ambient
{
	double		ratio;
	t_color3	color;
} t_ambient;

typedef struct s_light
{
	t_point3	position;
	double		brightness;
	t_color3	color;
} t_light;

typedef struct s_material
{
	t_color3	color;
} t_material;

typedef struct s_sphere
{
	t_point3	center;
	double		diameter;
	t_color3	color;
	t_material	material;
} t_sphere;

typedef struct s_plane
{
	t_point3	point;
	t_vec3		normal;
	t_color3	color;
	t_material	material;
} t_plane;

typedef struct s_cylinder
{
	t_point3	center;
	t_vec3		axis;
	double		diameter;
	double		height;
	t_color3	color;
	t_material	material;
} t_cylinder;

typedef struct s_cone
{
	t_point3	vertex;
	t_vec3		axis;
	double		angle;
	double		height;
	t_color3	color;
	t_material	material;
} t_cone;

typedef struct s_object
{
	int		type;
	union
	{
		t_sphere	sphere;
		t_plane		plane;
		t_cylinder	cylinder;
		t_cone		cone;
	} data;
} t_object;

typedef struct s_scene
{
	t_camera	camera;
	t_ambient	ambient;
	t_light		light;
	t_object	objects[MAX_OBJECTS];
	int		num_objects;
	int		has_ambient;
	int		has_light;
} t_scene;

#endif
