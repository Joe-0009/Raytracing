#include "../includes/math_utils_bonus.h"
#include <math.h>

double	solve_quadratic(double a, double b, double c, double min_t)
{
	double	discriminant;
	double	sqrt_d;
	double	t0;
	double	t1;

	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (-1.0);
	sqrt_d = sqrt(discriminant);
	t0 = (-b - sqrt_d) / (2.0 * a);
	t1 = (-b + sqrt_d) / (2.0 * a);
	if (t0 > min_t && (t0 < t1 || t1 <= min_t))
		return (t0);
	if (t1 > min_t)
		return (t1);
	return (-1.0);
}
