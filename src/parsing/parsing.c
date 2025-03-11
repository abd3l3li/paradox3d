/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-baz <abel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:48:53 by abel-baz          #+#    #+#             */
/*   Updated: 2025/03/10 23:40:54 by abel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

void	drain_gnl(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (line != NULL)
	{
		free(line);
		line = get_next_line(fd);
	}
}

void	free_splited(char **splited)
{
	int	i;

	i = 0;
	while (splited[i])
	{
		free(splited[i]);
		i++;
	}
	free(splited);
}

int	empty_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ')
		{
			if (line[i] == '\n')
				return (1);
			else
				return (0);
		}
		i++;
	}
	return (1);
}

int	extension_check(char *file)
{
	int	i;

	i = 0;
	while (file[i])
		i++;
	if (i < 5)
		return (-1);
	if (file[i - 1] != 'b' || file[i - 2] != 'u' || file[i - 3] != 'c' || file[i
			- 4] != '.')
		return (-1);
	return (0);
}

int	parse_cub(char *file, t_cub *cub)
{
	cub->line = get_next_line(cub->fd);
	if (extension_check(file) == -1)
		return (-1);
	if (parse_texture_color(cub) == -1)
		return (-1);
	if (parse_map(cub) == -1)
		return (-1);
	while (cub->line)
	{
		if (!empty_line(cub->line))
		{
			free(cub->line);
			cub->line = NULL;
			return (-1);
		}
		cub->line = get_next_line(cub->fd);
	}
	free(cub->line);
	return (0);
}
