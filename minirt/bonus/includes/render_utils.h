#ifndef RENDER_UTILS_H
# define RENDER_UTILS_H

# include "minirt_app.h"
# include "scene_math.h"

/* Color utilities */
int			color_to_int(t_color3 color);
int			get_sky_color(t_ray ray);
t_color3	clamp_color(t_color3 color);

/* Drawing utilities */
void		create_image(t_vars *vars);
void		put_pixel(t_vars *vars, int x, int y, int color);
void		main_draw(t_vars *vars, t_scene *scene);

/* Lighting utilities */
t_color3	calculate_ambient(const t_scene *scene, const t_hit *hit);
t_color3	calculate_diffuse(const t_scene *scene, const t_hit *hit);
int			is_in_shadow(const t_scene *scene, const t_vec3 point,
				const t_vec3 light_pos);
t_color3	calculate_lighting(const t_scene *scene, const t_hit *hit);

#endif
