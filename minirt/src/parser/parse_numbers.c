#include "../includes/minirt.h"
#include "../includes/parser.h"

int	parse_double(char *str, double *value)
{
	char	*dot;
	double	result;
	int		divisor;
	int		j;

	if (!str || !value)
		return (FALSE);
	dot = ft_strchr(str, '.');
	if (dot)
	{
		*dot = '\0';
		result = (double)ft_atoi(str);
		divisor = 1;
		j = -1;
		while (dot[++j + 1])
			divisor *= 10;
		result += (double)ft_atoi(dot + 1) / divisor;
	}
	else
		result = (double)ft_atoi(str);
	*value = result;
	return (TRUE);
}
