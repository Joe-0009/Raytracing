/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:51:57 by yrachidi          #+#    #+#             */
/*   Updated: 2024/10/31 13:52:58 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_tokens(const char *s, const char *delims)
{
	int count = 0;
	int i = 0;
	while (s[i])
	{
		while (s[i] && ft_strchr(delims, s[i]))
			i++;
		if (s[i])
		{
			count++;
			while (s[i] && !ft_strchr(delims, s[i]))
				i++;
		}
	}
	return (count);
}

static int	fill_split(const char *s, const char *delims, char **result, int count)
{
	int i = 0, j = 0, k;
	while (j < count)
	{
		while (s[i] && ft_strchr(delims, s[i]))
			i++;
		k = i;
		while (s[k] && !ft_strchr(delims, s[k]))
			k++;
		result[j] = (char *)malloc(sizeof(char) * (k - i + 1));
		if (!result[j])
		{
			while (--j >= 0)
				free(result[j]);
			free(result);
			return (0);
		}
		ft_strlcpy(result[j], &s[i], k - i + 1);
		i = k;
		j++;
	}
	result[count] = NULL;
	return (1);
}

char	**ft_split(const char *s, const char *delims)
{
	char **result;
	int count;

	if (!s || !delims)
		return (NULL);
	count = count_tokens(s, delims);
	result = (char **)malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	if (!fill_split(s, delims, result, count))
		return (NULL);
	return (result);
}
