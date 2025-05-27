#include "libft.h"
#include <stdarg.h>
#include <stdio.h>


static int	append_char_to_result(char **result, char c)
{
	char	*temp;
	size_t	curr_len;

	if (!result || !*result)
		return (-1);
	curr_len = ft_strlen(*result);
	temp = *result;
	*result = ft_calloc(curr_len + 2, sizeof(char));
	if (!*result)
	{
		ft_free((void **)&temp);
		return (-1);
	}
	ft_strlcpy(*result, temp, curr_len + 1);
	(*result)[curr_len] = c;
	ft_free((void **)&temp);
	return (1);
}

static int	append_str_to_result(char **result, const char *str)
{
	char	*temp;
	size_t	len;

	if (!result || !*result)
		return (-1);
	if (!str)
		str = "(null)";
	len = ft_strlen(str);
	temp = *result;
	*result = ft_strjoin(*result, str);
	if (!*result)
	{
		ft_free((void **)&temp);
		return (-1);
	}
	ft_free((void **)&temp);
	return (len);
}

static int	handle_int_specifier(char **result, va_list args)
{
	char	*num_str;
	int		num;
	int		len;

	num = va_arg(args, int);
	num_str = ft_itoa(num);
	if (!num_str)
		return (-1);
	len = append_str_to_result(result, num_str);
	ft_free((void **)&num_str);
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
		return (append_str_to_result(result, str));
	}
	else if (specifier == 'c')
	{
		c = (char)va_arg(args, int);
		return (append_char_to_result(result, c));
	}
	else if (specifier == 'd' || specifier == 'i')
	{
		return (handle_int_specifier(result, args));
	}
	else if (specifier == '%')
	{
		return (append_char_to_result(result, '%'));
	}
	if (append_char_to_result(result, '%') == -1
		|| append_char_to_result(result, specifier) == -1)
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
		if (append_char_to_result(result, format[*i]) == -1)
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
			ft_free((void **)result);
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

	if (!format)
		return (-1);
	va_start(args, format);
	result = ft_strdup("");
	if (!result)
	{
		va_end(args);
		return (-1);
	}
	count = process_format_string(&result, format, args);
	va_end(args);
	if (count == -1)
		return (-1);
	ft_putstr_fd(result, fd);
	ft_free((void **)&result);
	return (count);
}
