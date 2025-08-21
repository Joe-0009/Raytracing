/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:29:42 by isallali          #+#    #+#             */
/*   Updated: 2025/08/21 21:29:43 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/math_utils.h"
#include <math.h>

/**
 * Helper function to solve quadratic equation ax^2 + bx + c = 0
 * Returns the smallest positive root > min_t, or -1 if none
 */
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
