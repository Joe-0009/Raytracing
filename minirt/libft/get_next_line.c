/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:41:16 by yrachidi          #+#    #+#             */
/*   Updated: 2024/12/04 18:33:10 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free(char **buffer)
{
	if (*buffer)
	{
		free(*buffer);
		*buffer = NULL;
	}
}

char	*get_after(char *buffer)
{
	char	*line_location;
	char	*after;
	size_t	len;

	line_location = ft_strchr(buffer, '\n');
	if (!line_location)
		return (NULL);
	line_location++;
	len = ft_strlen(line_location);
	after = ft_calloc(len + 1, 1);//had ster
	if (!after)
		return (ft_free(&buffer), NULL);
	ft_memcpy(after, line_location, len);
	return (after);
}

char	*get_before(char *buffer)
{
	char	*before;
	int		i;

	i = 0;
	if (!buffer)
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	before = ft_calloc(i + 1, 1);
	if (!before)
		return (NULL);
	ft_memcpy(before, buffer, i);
	return (before);
}

char	*get_full_line(int fd, char *rbuf)
{
	int		return_bytes;
	char	*buffer;
	char	*tmp;

	buffer = ft_calloc((size_t)(BUFFER_SIZE + 1), 1);
	if (!buffer)
		return (ft_free(&rbuf), NULL);
	while (1)
	{
		return_bytes = read(fd, buffer, BUFFER_SIZE);
		if (return_bytes < 0)
			return (ft_free(&buffer), ft_free(&rbuf), NULL);
		if (return_bytes == 0)
			break ;
		buffer[return_bytes] = '\0';
		tmp = ft_strjoin(rbuf, buffer);//had ster
		ft_free(&rbuf);
		rbuf = tmp;
		if (!rbuf)
			return (ft_free(&buffer), NULL);
		if (ft_strchr(rbuf, '\n'))
			break ;
	}
	ft_free(&buffer);
	return (rbuf);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;
	char 		*remaining;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE >= INT_MAX)
		return (NULL);
	buffer = get_full_line(fd, buffer);
	if (!buffer)
		return (NULL);
	if (buffer && !*buffer)
	{
		ft_free(&buffer);//had ster
		return (NULL);
	}
	if (ft_strchr(buffer, '\n'))
	{
		line = get_before(buffer);
		remaining = get_after(buffer);
		ft_free(&buffer);
		buffer = remaining;
		return (line);
	}
	line = buffer;
	buffer = NULL;
	return (line);
}

