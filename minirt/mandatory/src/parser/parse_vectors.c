#include "../includes/minirt_app.h"
#include "../includes/parser.h"
#include <math.h>
#include <stdlib.h>

static int	parse_vector_tokens(char **tokens, t_vec3 *vec)
{
	double	x;
	double	y;
	double	z;

	if (!parse_double(tokens[0], &x) || !parse_double(tokens[1], &y)
		|| !parse_double(tokens[2], &z))
		return (FALSE);
	vec->x = x;
	vec->y = y;
	vec->z = z;
	return (TRUE);
}

int	parse_vector(char *str, t_vec3 *vec)
{
	char	**tokens;
	int		success;

	tokens = ft_split(str, ",");
	if (!tokens || !tokens[0] || !tokens[1] || !tokens[2])
	{
		printf(ERR_VECTOR_FORMAT);
		free_tokens(tokens);
		return (FALSE);
	}
	success = parse_vector_tokens(tokens, vec);
	if (!success)
		printf(ERR_VECTOR_FORMAT);
	free_tokens(tokens);
	return (success);
}

int	validate_non_zero_vector(t_vec3 vec)
{
	if (vec.x == 0.0 && vec.y == 0.0 && vec.z == 0.0)
	{
		printf(ERR_VECTOR_FORMAT);
		return (FALSE);
	}
	return (TRUE);
}

int	validate_normalized_vector(t_vec3 vec)
{
	double	length;

	length = vec3_length(vec);
	if (fabs(length - 1.0) > 0.0001)
	{
		printf(ERR_VECTOR_FORMAT);
		return (FALSE);
	}
	return (TRUE);
}
