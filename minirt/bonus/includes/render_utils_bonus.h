#ifndef RENDER_UTILS_H
# define RENDER_UTILS_H

# include "minirt_app_bonus.h"
# include "scene_bonus.h"

/* Forward declaration for optimization struct */
typedef struct s_light_data
{
	t_vec3		light_dir;
	double		distance;
	double		attenuation;
	int			in_shadow;
	double		normal_dot_light;
}				t_light_data;

/* Color utilities */
int			color_to_int(t_color3 color);
int			get_sky_color(t_ray ray);
t_color3	clamp_color(t_color3 color);

/* Drawing utilities */
void		create_image(t_vars *vars);
void		put_pixel(t_vars *vars, int x, int y, int color);
void		main_draw(t_vars *vars, t_scene *scene);

/* Lighting utilities */
t_color3	calculate_diffuse_with_data(const t_light *light, const t_hit *hit, const t_light_data *data);
t_color3	calculate_specular_with_data(const t_light *light, const t_hit *hit, 
				const t_vec3 view_dir, const t_light_data *data);
int			is_in_shadow (const t_scene *scene, const t_vec3 point,
				const t_vec3 light_pos, const t_vec3 light_dir, double light_distance);
t_color3	calculate_phong_lighting(const t_scene *scene, const t_hit *hit, const t_vec3 view_dir);

#endif
