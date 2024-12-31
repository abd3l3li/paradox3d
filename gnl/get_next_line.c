/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-baz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 21:17:16 by abel-baz          #+#    #+#             */
/*   Updated: 2023/12/08 21:40:14 by abel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_reads(int fd, char *str)
{
	char	*buf;
	int		count;

	buf = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	count = 1;
	while (!ft_strchr(str, '\n') && count != 0)
	{
		count = read(fd, buf, BUFFER_SIZE);
		if (count == -1)
			return (free_buf(buf));
		buf[count] = '\0';
		str = ft_strjoinn(str, buf);
	}
	free(buf);
	return (str);
}

static char	*ft_extract_line(char *str)
{
	int		i;
	char	*dup;

	i = 0;
	if (!str[i])
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	dup = (char *)malloc(sizeof(char) * (i + 2));
	if (!dup)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
	{
		dup[i] = str[i];
		i++;
	}
	if (str[i] == '\n')
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

static char	*ft_next_update(char *str)
{
	int		i;
	int		j;
	char	*dup;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (!str[i])
		return (free_buf(str));
	dup = (char *)malloc(sizeof(char) * (ft_strlen(str) - i + 1));
	if (!dup)
		return (free_buf(str));
	i++;
	j = 0;
	while (str[i])
		dup[j++] = str[i++];
	dup[j] = '\0';
	free(str);
	return (dup);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*str;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	str = ft_reads(fd, str);
	if (!str)
		return (NULL);
	line = ft_extract_line(str);
	str = ft_next_update(str);
	return (line);
}
