#include "../includes/minirt.h"

/**
 * Print an error message to stderr
 */
void	ft_print_error(const char *message)
{
	if (message)
		ft_putstr_fd((char *)message, STDERR_FILENO);
}

/**
 * Print an error message with additional details to stderr
 */
void	ft_print_error_detail(const char *message, const char *detail)
{
	ft_print_error(message);
	if (detail)
		ft_putstr_fd((char *)detail, STDERR_FILENO);
}

/**
 * Print a number to stderr (simplified version)
 */
void	ft_print_number(double num)
{
	char	*str;

	str = ft_itoa((int)(num * 1000000));
	if (str)
	{
		ft_putstr_fd(str, STDERR_FILENO);
		free(str);
	}
}

/**
 * Print a vector to stderr (for error messages)
 */
void	ft_print_vector(t_vec3 v)
{
	ft_putstr_fd("(", STDERR_FILENO);
	ft_print_number(v.x);
	ft_putstr_fd(", ", STDERR_FILENO);
	ft_print_number(v.y);
	ft_putstr_fd(", ", STDERR_FILENO);
	ft_print_number(v.z);
	ft_putstr_fd(")", STDERR_FILENO);
}
