/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:47:20 by yrachidi          #+#    #+#             */
/*   Updated: 2025/04/18 16:49:15 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	ft_number_of_divisions(unsigned int n, int base)
{
	unsigned int	num;

	if (base < 2)
		return (-1);
	if (n == 0)
		return (1);
	num = 0;
	while (n > 0)
	{
		n /= base;
		num++;
	}
	return (num);
}

char	*ft_itoa(int n)
{
	char			*integer;
	unsigned int	num;
	int				len;

	num = n;
	if (n < 0)
		num = -n;
	len = ft_number_of_divisions(num, 10);
	if (n < 0)
		len++;
	integer = malloc(len + 1);
	if (!integer)
		return (NULL);
	integer[len] = '\0';
	if (n == 0)
		integer[0] = '0';
	if (n < 0)
		integer[0] = '-';
	while (num)
	{
		integer[--len] = num % 10 + '0';
		num /= 10;
	}
	return (integer);
}
