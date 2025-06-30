/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youssefrachidi <youssefrachidi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:51:57 by yrachidi          #+#    #+#             */
/*   Updated: 2025/06/05 17:31:10 by youssefrach      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_tokens(const char *s, const char *delims)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
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

static int	fill_split(const char *s, const char *delims, char **result,
		int count)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	while (j < count)
	{
		while (s[i] && ft_strchr(delims, s[i]))
			i++;
		k = i;
		while (s[k] && !ft_strchr(delims, s[k]))
			k++;
		result[j] = (char *)malloc(sizeof(char) * (k - i + 1));
		ft_strlcpy(result[j], &s[i], k - i + 1);
		i = k;
		j++;
	}
	result[count] = NULL;
	return (1);
}

char	**ft_split(const char *s, const char *delims)
{
	char	**result;
	int		count;

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
