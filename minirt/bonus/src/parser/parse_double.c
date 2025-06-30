#include "../includes/minirt_app.h"
#include "../includes/parser.h"

int	parse_double(char *str, double *value)
{
	char	*dot;
	double	result;
	char	*frac_str;
	int		divisor;
	int		j;

	if (!str || !value)
		return (FALSE);
	dot = ft_strchr(str, '.');
	result = 0.0;
	if (dot)
	{
		*dot = '\0';
		result = (double)ft_atoi(str);
		frac_str = dot + 1;
		divisor = 1;
		j = -1;
		while (frac_str[++j])
			divisor *= 10;
		result += (double)ft_atoi(frac_str) / divisor;
	}
	else
		result = (double)ft_atoi(str);
	*value = result;
	return (TRUE);
}
