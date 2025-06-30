#include "../includes/minirt_app.h"
#include "../includes/parser.h"

int	parse_color(char *str, t_color3 *color)
{
	char	**tokens;
	int		success;
	int		r;
	int		g;
	int		b;

	success = TRUE;
	tokens = ft_split(str, ",");
	if (!tokens || !tokens[0] || !tokens[1] || !tokens[2])
		return (printf(ERR_COLOR_FORMAT), free_tokens(tokens), FALSE);
	r = ft_atoi(tokens[0]);
	g = ft_atoi(tokens[1]);
	b = ft_atoi(tokens[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		success = FALSE;
	if (success)
	{
		color->x = r / 255.0;
		color->y = g / 255.0;
		color->z = b / 255.0;
	}
	else
		printf(ERR_COLOR_FORMAT);
	free_tokens(tokens);
	return (success);
}
