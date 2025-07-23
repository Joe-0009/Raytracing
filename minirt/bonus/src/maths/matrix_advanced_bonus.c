#include "../includes/math_utils_bonus.h"
#include <math.h>

/*
** Create rotation matrix using Rodrigues' rotation formula
** Rotates around arbitrary axis by given angle
*/
t_matrix4	matrix4_rotation_axis(t_vec3 axis, double angle)
{
	t_matrix4	m;
	t_vec3		k;
	double		cos_a;
	double		sin_a;
	double		one_minus_cos;

	k = vec3_normalize(axis);
	cos_a = cos(angle);
	sin_a = sin(angle);
	one_minus_cos = 1.0 - cos_a;

	// Identity matrix
	m = matrix4_identity();

	// Rodrigues' rotation formula components
	// R = I + sin(θ)K + (1-cos(θ))K²
	// where K is the skew-symmetric cross-product matrix of axis k

	// Diagonal elements
	m.m[0][0] = cos_a + k.x * k.x * one_minus_cos;
	m.m[1][1] = cos_a + k.y * k.y * one_minus_cos;
	m.m[2][2] = cos_a + k.z * k.z * one_minus_cos;

	// Off-diagonal elements
	m.m[0][1] = k.x * k.y * one_minus_cos - k.z * sin_a;
	m.m[0][2] = k.x * k.z * one_minus_cos + k.y * sin_a;
	m.m[1][0] = k.y * k.x * one_minus_cos + k.z * sin_a;
	m.m[1][2] = k.y * k.z * one_minus_cos - k.x * sin_a;
	m.m[2][0] = k.z * k.x * one_minus_cos - k.y * sin_a;
	m.m[2][1] = k.z * k.y * one_minus_cos + k.x * sin_a;

	return (m);
}

/*
** Create a full transformation matrix from translation, rotation, and scale
** Order: Scale -> Rotate -> Translate (standard order)
*/
t_matrix4	matrix4_transform(t_vec3 translation, t_vec3 rotation, t_vec3 scale)
{
	t_matrix4	scale_matrix;
	t_matrix4	rotation_x;
	t_matrix4	rotation_y;
	t_matrix4	rotation_z;
	t_matrix4	rotation_combined;
	t_matrix4	translation_matrix;
	t_matrix4	result;

	// Create component matrices
	scale_matrix = matrix4_scale(scale);
	rotation_x = matrix4_rotation_x(rotation.x);
	rotation_y = matrix4_rotation_y(rotation.y);
	rotation_z = matrix4_rotation_z(rotation.z);
	translation_matrix = matrix4_translation(translation);

	// Combine rotations: Z * Y * X
	rotation_combined = matrix4_multiply(rotation_z, rotation_y);
	rotation_combined = matrix4_multiply(rotation_combined, rotation_x);

	// Final transformation: T * R * S
	result = matrix4_multiply(rotation_combined, scale_matrix);
	result = matrix4_multiply(translation_matrix, result);

	return (result);
}

/*
** Decompose a transformation matrix into translation, rotation, scale
** Note: This is a simplified decomposition for educational purposes
*/
void	matrix4_decompose(t_matrix4 matrix, t_vec3 *translation,
			t_vec3 *rotation, t_vec3 *scale)
{
	// Extract translation (last column)
	translation->x = matrix.m[0][3];
	translation->y = matrix.m[1][3];
	translation->z = matrix.m[2][3];

	// Extract scale (length of first three columns)
	scale->x = sqrt(matrix.m[0][0] * matrix.m[0][0] +
					matrix.m[1][0] * matrix.m[1][0] +
					matrix.m[2][0] * matrix.m[2][0]);
	scale->y = sqrt(matrix.m[0][1] * matrix.m[0][1] +
					matrix.m[1][1] * matrix.m[1][1] +
					matrix.m[2][1] * matrix.m[2][1]);
	scale->z = sqrt(matrix.m[0][2] * matrix.m[0][2] +
					matrix.m[1][2] * matrix.m[1][2] +
					matrix.m[2][2] * matrix.m[2][2]);

	// Extract rotation (simplified - assumes no skew)
	// This is a basic implementation - full rotation extraction is more complex
	rotation->x = 0;
	rotation->y = 0;
	rotation->z = 0;
}

/*
** Create look-at matrix for camera positioning
*/
t_matrix4	matrix4_look_at(t_vec3 eye, t_vec3 target, t_vec3 up)
{
	t_vec3		forward;
	t_vec3		right;
	t_vec3		camera_up;
	t_matrix4	result;

	// Calculate camera coordinate system
	forward = vec3_normalize(vec3_sub(target, eye));
	right = vec3_normalize(vec3_cross(forward, up));
	camera_up = vec3_cross(right, forward);

	// Create look-at matrix
	result = matrix4_identity();

	// Set rotation part
	result.m[0][0] = right.x;
	result.m[1][0] = right.y;
	result.m[2][0] = right.z;
	result.m[0][1] = camera_up.x;
	result.m[1][1] = camera_up.y;
	result.m[2][1] = camera_up.z;
	result.m[0][2] = -forward.x;
	result.m[1][2] = -forward.y;
	result.m[2][2] = -forward.z;

	// Set translation part
	result.m[0][3] = -vec3_dot(right, eye);
	result.m[1][3] = -vec3_dot(camera_up, eye);
	result.m[2][3] = vec3_dot(forward, eye);

	return (result);
}

/*
** Rotate a vector using matrix transformation (for compatibility)
*/
t_vec3	matrix_rotate_vector(t_vec3 vector, t_vec3 axis, double angle)
{
	t_matrix4	rotation_matrix;

	rotation_matrix = matrix4_rotation_axis(axis, angle);
	return (matrix4_transform_direction(rotation_matrix, vector));
}
