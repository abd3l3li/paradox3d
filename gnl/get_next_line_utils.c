/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-baz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 21:40:36 by abel-baz          #+#    #+#             */
/*   Updated: 2023/12/08 21:57:26 by abel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*free_buf(char *buf)
{
	free(buf);
	return (NULL);
}

char	*ft_strjoinn(char *str, char *buf)
{
	size_t	i;
	size_t	j;
	char	*result;

	if (!str)
	{
		str = (char *)malloc(1 * sizeof(char));
		if (!str)
			return (0);
		str[0] = '\0';
	}
	if (!str || !buf)
		return (NULL);
	result = malloc(sizeof(char) * ((ft_strlen(str) + ft_strlen(buf)) + 1));
	if (!result)
		return (free_buf(str));
	i = -1;
	j = 0;
	while (str[++i] != '\0')
		result[i] = str[i];
	while (buf[j] != '\0')
		result[i++] = buf[j++];
	result[ft_strlen(str) + ft_strlen(buf)] = '\0';
	free(str);
	return (result);
}
