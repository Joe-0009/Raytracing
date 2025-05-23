#include "../includes/main.h"
#include <math.h>

// Helper struct for a ray
typedef struct s_ray
{
	t_vec3		origin;
	t_vec3		direction;
}				t_ray;

// Generate a ray from the camera through pixel (x, y)
t_ray	generate_camera_ray(const t_scene *scene, int x, int y)
{
	//todo
}

// Ray-sphere intersection: returns t if hit, -1 if no hit
double	hit_sphere(const t_sphere *sphere, t_ray ray)
{
	// todo
}

// Ray-plane intersection: returns t if hit, -1 if no hit
double	hit_plane(const t_plane *plane, t_ray ray)
{
	// todo
}

// Ray-cylinder intersection: returns t if hit, -1 if no hit
// Now supports finite height and caps
// Returns the closest t for side or caps

double	hit_cylinder(const t_cylinder *cyl, t_ray ray)
{
	// todo
}

// Compute diffuse + specular lighting (Lambertian+ Blinn-Phong) with shadow checks
// Returns total intensity (0..1), and sets *specular (0..1) if not NULL
// hit_type/hit_obj are used to avoid self-shadowing
// view_dir is the direction from point to camera

double	compute_lighting(const t_scene *scene, t_vec3 point, t_vec3 normal,
		t_vec3 view_dir, int hit_type, void *hit_obj, double *specular_out)
{
	// todo
}

// Helper to get normal at hit point for each object
// Now returns correct normal for cylinder caps

t_vec3	get_normal(int type, void *obj, t_vec3 hit_point)
{
	// todo
}

// Reflect a vector v around normal n
static t_vec3	reflect(t_vec3 v, t_vec3 n)
{
	return (vec3_sub(v, vec3_mult(n, 2 * vec3_dot(v, n))));
}

// Add a max_depth parameter for recursion
int	trace_ray_recursive(const t_scene *scene, t_ray ray, int depth)
{
	// todo
}

// Replace trace_ray with a wrapper for recursion
int	trace_ray(const t_scene *scene, t_ray ray)
{
	return (trace_ray_recursive(scene, ray, 2));
	// 2 = max recursion depth (1 bounce)
}

void	create_image(t_vars *vars)
{
	vars->img = malloc(sizeof(t_image));
	if (!vars->img)
		exit(EXIT_FAILURE);
	vars->img->img = mlx_new_image(vars->mlx, WIDTH, HEIGHT);
	if (!vars->img->img)
		exit(EXIT_FAILURE);
	vars->img->addr = mlx_get_data_addr(vars->img->img,
			&vars->img->bits_per_pixel, &vars->img->line_length,
			&vars->img->endian);
	if (!vars->img->addr)
		exit(EXIT_FAILURE);
}
void	put_pixel(t_vars *vars, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		dst = vars->img->addr + (y * vars->img->line_length + x
				* (vars->img->bits_per_pixel / 8));
		*(unsigned int *)dst = color;
	}
}

// Simple pixel-by-pixel rendering loop for miniRT
// This is a stub. You should replace the color calculation with ray tracing logic.
void	main_draw(t_vars *vars, t_scene *scene)
{
	t_ray	ray;

	int x, y, color;
	for (y = 0; y < HEIGHT; y++)
	{
		for (x = 0; x < WIDTH; x++)
		{
			ray = generate_camera_ray(scene, x, y);
			color = trace_ray(scene, ray);
			put_pixel(vars, x, y, color);
		}
	}
}
