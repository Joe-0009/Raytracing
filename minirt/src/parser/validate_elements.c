#include <math.h>
#include "../includes/minirt.h"

int	validate_plane_normal(t_vec3 *normal)
{
	if (!validate_non_zero_vector(*normal))
	{
		ft_fprintf_fd(2, ERR_PLANE_FORMAT);
		return (FALSE);
	}
	*normal = vec3_normalize(*normal);
	if (!validate_normalized_vector(*normal))
	{
		ft_fprintf_fd(2, ERR_PLANE_FORMAT);
		return (FALSE);
	}
	return (TRUE);
}

int	validate_sphere_diameter(double diameter)
{
	if (diameter <= 0.0)
	{
		ft_fprintf_fd(2, ERR_SPHERE_FORMAT);
		ft_fprintf_fd(2, "Sphere diameter must be positive\n");
		return (FALSE);
	}
	if (diameter < 0.001)
	{
		ft_fprintf_fd(2, "Warning: Very small sphere diameter (%.6f) may cause "
			"rendering issues\n", diameter);
	}
	return (TRUE);
}

int	validate_cylinder_dimensions(double diameter, double height)
{
	if (diameter <= 0.0 || height <= 0.0)
	{
		ft_fprintf_fd(2, ERR_CYLINDER_FORMAT);
		ft_fprintf_fd(2, "Cylinder diameter and height must be positive\n");
		return (FALSE);
	}
	if (diameter < 0.001)
	{
		ft_fprintf_fd(2, "Warning: Very small cylinder diameter (%.6f) may cause "
			"rendering issues\n", diameter);
	}
	if (height < 0.001)
	{
		ft_fprintf_fd(2, "Warning: Very small cylinder height (%.6f) may cause "
			"rendering issues\n", height);
	}
	return (TRUE);
}

int	validate_cone_dimensions(double angle, double height)
{
	if (angle <= 0.0 || height <= 0.0)
	{
		ft_fprintf_fd(2, ERR_CONE_FORMAT);
		ft_fprintf_fd(2, "Cone angle and height must be positive\n");
		return (FALSE);
	}
	if (angle > M_PI)
	{
		ft_fprintf_fd(2, ERR_CONE_FORMAT);
		ft_fprintf_fd(2, "Cone angle must be less than or equal to 180 degrees\n");
		return (FALSE);
	}
	if (angle < 0.01)
	{
		ft_fprintf_fd(2, "Warning: Very small cone angle (%.6f radians) may cause "
			"rendering issues\n", angle);
	}
	if (height < 0.001)
	{
		ft_fprintf_fd(2, "Warning: Very small cone height (%.6f) may cause "
			"rendering issues\n", height);
	}
	return (TRUE);
}

int	validate_sphere(t_sphere *sphere)
{
	if (!validate_position(sphere->center, "Sphere"))
		return (FALSE);
	if (sphere->diameter <= 0.0)
	{
		ft_fprintf_fd(2, "Error: Sphere diameter must be positive\n");
		return (FALSE);
	}
	if (sphere->diameter < 0.1)
		ft_fprintf_fd(2, "Warning: Sphere diameter is very small\n");
	return (TRUE);
}

int	validate_cylinder(t_cylinder *cylinder)
{
	if (!validate_position(cylinder->center, "Cylinder"))
		return (FALSE);
	if (!validate_non_zero_vector(cylinder->axis))
	{
		ft_fprintf_fd(2, "Error: Cylinder axis vector cannot be (0,0,0)\n");
		return (FALSE);
	}
	if (!validate_normalized_vector(cylinder->axis))
	{
		ft_fprintf_fd(2, "Error: Cylinder axis must be normalized\n");
		return (FALSE);
	}
	if (cylinder->diameter <= 0.0 || cylinder->height <= 0.0)
	{
		ft_fprintf_fd(2, "Error: Cylinder dimensions must be positive\n");
		return (FALSE);
	}
	if (cylinder->height < 0)
	{
		ft_fprintf_fd(2, "Error: Cylinder height cannot be negative\n");
		return (FALSE);
	}
	if (cylinder->diameter < 0.1 || cylinder->height < 0.1)
		ft_fprintf_fd(2, "Warning: Cylinder dimensions are very small\n");
	return (TRUE);
}

int	validate_plane(t_plane *plane)
{
	if (!validate_position(plane->point, "Plane"))
		return (FALSE);
	if (!validate_non_zero_vector(plane->normal))
	{
		ft_fprintf_fd(2, "Error: Plane normal vector cannot be (0,0,0)\n");
		return (FALSE);
	}
	if (!validate_normalized_vector(plane->normal))
	{
		ft_fprintf_fd(2, "Error: Plane normal must be normalized\n");
		return (FALSE);
	}
	return (TRUE);
}

int	validate_position(t_point3 pos, const char *type)
{
	double	dist;

	dist = sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
	if (dist > 1000.0)
	{
		ft_fprintf_fd(2, "Warning: %s position is far from the origin (%.2f, %.2f, %.2f)\n",
			type, pos.x, pos.y, pos.z);
	}
	return (1);
}