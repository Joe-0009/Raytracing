#include "libft.h"
#include <stdarg.h>

static int	handle_format(int fd, const char *format, va_list args)
{
	int	count;

	count = 0;
	if (*format == 'c')
	{
		ft_putchar_fd(va_arg(args, int), fd);
		count++;
	}
	else if (*format == 's')
	{
		char	*str;

		str = va_arg(args, char *);
		if (!str)
			str = "(null)";
		ft_putstr_fd(str, fd);
		count += ft_strlen(str);
	}
	else if (*format == 'd' || *format == 'i')
	{
		char	*num;
		int		n;

		n = va_arg(args, int);
		num = ft_itoa(n);
		if (!num)
			return (-1);
		ft_putstr_fd(num, fd);
		count += ft_strlen(num);
		free(num);
	}
	else if (*format == '%')
	{
		ft_putchar_fd('%', fd);
		count++;
	}
	return (count);
}

int	ft_fprintf_fd(int fd, const char *format, ...)
{
	va_list	args;
	int		count;
	int		temp;

	if (!format)
		return (-1);
	va_start(args, format);
	count = 0;
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			temp = handle_format(fd, format, args);
			if (temp == -1)
			{
				va_end(args);
				return (-1);
			}
			count += temp;
		}
		else
		{
			ft_putchar_fd(*format, fd);
			count++;
		}
		format++;
	}
	va_end(args);
	return (count);
} 