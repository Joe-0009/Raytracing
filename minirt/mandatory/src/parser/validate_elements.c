#include "../includes/minirt_app.h"
#include <math.h>
#include <stdio.h>

int	validate_plane_normal(t_vec3 *normal)
{
	if (!validate_non_zero_vector(*normal))
		return (printf(ERR_PLANE_FORMAT), FALSE);
	*normal = vec3_normalize(*normal);
	if (!validate_normalized_vector(*normal))
		return (printf(ERR_PLANE_FORMAT), FALSE);
	return (TRUE);
}

int	validate_cone_dimensions(double angle, double height)
{
	if (angle <= 0.0 || height <= 0.0)
		return (printf(ERR_CONE_FORMAT),
			printf(ERR_CONE_DIMS_POSITIVE), FALSE);
	if (angle > M_PI)
		return (printf(ERR_CONE_FORMAT),
			printf(ERR_CONE_ANGLE_TOO_LARGE), FALSE);
	if (angle < 0.01)
	{
		printf(WARN_CONE_ANGLE_SMALL, angle);
	}
	if (height < 0.001)
	{
		printf(WARN_CONE_HEIGHT_SMALL, height);
	}
	return (TRUE);
}

int	validate_sphere(t_sphere *sphere)
{
	if (!validate_position(sphere->center, "Sphere"))
		return (FALSE);
	if (sphere->diameter <= 0.0)
		return (printf(ERR_SPHERE_FORMAT),
			printf(ERR_SPHERE_DIAMETER_POSITIVE), FALSE);
	if (sphere->diameter < 0.001)
		printf(WARN_SPHERE_DIAMETER_SMALL);
	else if (sphere->diameter < 0.1)
		printf(WARN_SPHERE_DIAMETER_VERY_SMALL);
	return (TRUE);
}

int	validate_cylinder(t_cylinder *cylinder)
{
	if (!validate_position(cylinder->center, "Cylinder"))
		return (FALSE);
	if (!validate_non_zero_vector(cylinder->axis))
		return (printf(ERR_CYLINDER_AXIS_ZERO), FALSE);
	if (!validate_normalized_vector(cylinder->axis))
		return (printf(ERR_CYLINDER_AXIS_NOT_NORMALIZED), FALSE);
	if (cylinder->diameter <= 0.0 || cylinder->height <= 0.0)
		return (printf(ERR_CYLINDER_FORMAT),
			printf(ERR_CYLINDER_DIMS_POSITIVE), FALSE);
	if (cylinder->height < 0)
		return (printf(ERR_CYLINDER_HEIGHT_NEGATIVE), FALSE);
	if (cylinder->diameter < 0.001)
		printf(WARN_CYLINDER_DIAMETER_SMALL);
	if (cylinder->height < 0.001)
		printf(WARN_CYLINDER_HEIGHT_SMALL);
	if (cylinder->diameter < 0.1 || cylinder->height < 0.1)
		printf(WARN_CYLINDER_DIMS_SMALL);
	return (TRUE);
}

int	validate_plane(t_plane *plane)
{
	if (!validate_position(plane->point, "Plane"))
		return (FALSE);
	if (!validate_non_zero_vector(plane->normal))
		return (printf(ERR_PLANE_NORMAL_ZERO), FALSE);
	if (!validate_normalized_vector(plane->normal))
		return (printf(ERR_PLANE_NORMAL_NOT_NORMALIZED), FALSE);
	return (TRUE);
}

int	validate_position(t_point3 pos, const char *type)
{
	double	dist;

	dist = sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
	if (dist > 1000.0)
		printf(WARN_POSITION_FAR, type, pos.x, pos.y, pos.z);
	return (1);
}
