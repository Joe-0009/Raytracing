#include "../includes/scene_math.h"

/*
** Create identity matrix
*/
t_matrix4	matrix4_identity(void)
{
	t_matrix4	m;
	int			i;
	int			j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			if (i == j)
				m.m[i][j] = 1.0;
			else
				m.m[i][j] = 0.0;
			j++;
		}
		i++;
	}
	return (m);
}

/*
** Multiply two 4x4 matrices
*/
t_matrix4	matrix4_multiply(t_matrix4 a, t_matrix4 b)
{
	t_matrix4	result;
	int			i;
	int			j;
	int			k;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			result.m[i][j] = 0.0;
			k = 0;
			while (k < 4)
			{
				result.m[i][j] += a.m[i][k] * b.m[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
	return (result);
}

/*
** Create translation matrix
*/
t_matrix4	matrix4_translation(t_vec3 translation)
{
	t_matrix4	m;

	m = matrix4_identity();
	m.m[0][3] = translation.x;
	m.m[1][3] = translation.y;
	m.m[2][3] = translation.z;
	return (m);
}

/*
** Create rotation matrix around X axis
*/
t_matrix4	matrix4_rotation_x(double angle)
{
	t_matrix4	m;
	double		cos_a;
	double		sin_a;

	m = matrix4_identity();
	cos_a = cos(angle);
	sin_a = sin(angle);
	m.m[1][1] = cos_a;
	m.m[1][2] = -sin_a;
	m.m[2][1] = sin_a;
	m.m[2][2] = cos_a;
	return (m);
}

/*
** Create rotation matrix around Y axis
*/
t_matrix4	matrix4_rotation_y(double angle)
{
	t_matrix4	m;
	double		cos_a;
	double		sin_a;

	m = matrix4_identity();
	cos_a = cos(angle);
	sin_a = sin(angle);
	m.m[0][0] = cos_a;
	m.m[0][2] = sin_a;
	m.m[2][0] = -sin_a;
	m.m[2][2] = cos_a;
	return (m);
}

/*
** Create rotation matrix around Z axis
*/
t_matrix4	matrix4_rotation_z(double angle)
{
	t_matrix4	m;
	double		cos_a;
	double		sin_a;

	m = matrix4_identity();
	cos_a = cos(angle);
	sin_a = sin(angle);
	m.m[0][0] = cos_a;
	m.m[0][1] = -sin_a;
	m.m[1][0] = sin_a;
	m.m[1][1] = cos_a;
	return (m);
}

/*
** Create scale matrix
*/
t_matrix4	matrix4_scale(t_vec3 scale)
{
	t_matrix4	m;

	m = matrix4_identity();
	m.m[0][0] = scale.x;
	m.m[1][1] = scale.y;
	m.m[2][2] = scale.z;
	return (m);
}

/*
** Transform a point by matrix (including translation)
*/
t_vec3	matrix4_transform_point(t_matrix4 m, t_vec3 point)
{
	t_vec3	result;

	result.x = m.m[0][0] * point.x + m.m[0][1] * point.y + m.m[0][2] * point.z + m.m[0][3];
	result.y = m.m[1][0] * point.x + m.m[1][1] * point.y + m.m[1][2] * point.z + m.m[1][3];
	result.z = m.m[2][0] * point.x + m.m[2][1] * point.y + m.m[2][2] * point.z + m.m[2][3];
	return (result);
}

/*
** Transform a direction by matrix (no translation)
*/
t_vec3	matrix4_transform_direction(t_matrix4 m, t_vec3 direction)
{
	t_vec3	result;

	result.x = m.m[0][0] * direction.x + m.m[0][1] * direction.y + m.m[0][2] * direction.z;
	result.y = m.m[1][0] * direction.x + m.m[1][1] * direction.y + m.m[1][2] * direction.z;
	result.z = m.m[2][0] * direction.x + m.m[2][1] * direction.y + m.m[2][2] * direction.z;
	return (vec3_normalize(result));
}
