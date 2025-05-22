#include "libft.h"
#include <stdarg.h>

static void	safe_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

static int	append_to_result(char **result, const char *str, char c,
		int is_char)
{
	char	*temp;
	size_t	curr_len;
	size_t	len;

	if (is_char)
	{
		curr_len = ft_strlen(*result);
		temp = *result;
		*result = ft_calloc(curr_len + 2, sizeof(char));
		if (!*result)
			return (safe_free((void **)&temp), -1);
		ft_strlcpy(*result, temp, curr_len + 1);
		(*result)[curr_len] = c;
		safe_free((void **)&temp);
		return (1);
	}
	if (!str)
		str = "(null)";
	len = ft_strlen(str);
	temp = *result;
	*result = ft_strjoin(*result, str);
	safe_free((void **)&temp);
	return (len);
}

static int	process_format_specifier(char **result, char specifier,
		va_list args)
{
	char	*str;
	char	c;

	if (specifier == 's')
	{
		str = va_arg(args, char *);
		return (append_to_result(result, str, 0, 0));
	}
	else if (specifier == 'c')
	{
		c = (char)va_arg(args, int);
		return (append_to_result(result, NULL, c, 1));
	}
	if (append_to_result(result, NULL, '%', 1) == -1 || append_to_result(result,
			NULL, specifier, 1) == -1)
		return (-1);
	return (2);
}

static int	process_format_char(char **result, const char *format, int *i,
		va_list args)
{
	int	ret;

	if (format[*i] == '%' && format[*i + 1])
	{
		(*i)++;
		ret = process_format_specifier(result, format[*i], args);
		if (ret == -1)
			return (-1);
		return (ret);
	}
	else
	{
		if (append_to_result(result, NULL, format[*i], 1) == -1)
			return (-1);
		return (1);
	}
}

static int	process_format_string(char **result, const char *format,
		va_list args)
{
	int	i;
	int	count;
	int	ret;

	count = 0;
	i = 0;
	while (format[i])
	{
		ret = process_format_char(result, format, &i, args);
		if (ret == -1)
		{
			safe_free((void **)&*result);
			va_end(args);
			return (-1);
		}
		count += ret;
		i++;
	}
	return (count);
}

int	ft_fprintf_fd(int fd, const char *format, ...)
{
	va_list	args;
	int		count;
	char	*result;

	va_start(args, format);
	result = ft_strdup("");
	if (!result)
		return (-1);
	count = process_format_string(&result, format, args);
	if (count == -1)
		return (-1);
	ft_putstr_fd(result, fd);
	safe_free((void **)&result);
	va_end(args);
	return (count);
}
