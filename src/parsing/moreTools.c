/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moreTools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-baz <abel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 23:54:31 by abel-baz          #+#    #+#             */
/*   Updated: 2025/03/10 23:56:08 by abel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

int	is_map(char *line, t_cub *cub)
{
	char	*tmp;

	tmp = ft_strtrim(line, " ");
	if (tmp[0] == '\0')
		return (0);
	if (cub->v_map->manner > 0)
	{
		if (tmp[0] == 'W' || tmp[0] == 'E' || tmp[0] == 'S' || tmp[0] == 'N')
			return (free(tmp), cub->v_map->manner--, 0);
	}
	if (tmp[0] == '1' || tmp[0] == '0' || tmp[0] == 'W' || tmp[0] == 'E'
		|| tmp[0] == 'S' || tmp[0] == 'N')
		return (free(tmp), 1);
	return (free(tmp), 0);
}

int	map_alloc(t_cub *cub)
{
	int		fd;
	char	*line;
	int		i;

	i = 0;
	fd = open(cub->v_map->argv, O_RDONLY);
	if (fd == -1)
		return (-1);
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (is_map(line, cub))
			i++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (i);
}

int	holy_checks(t_cub *cub)
{
	if (check_texture_counts(cub) == -1 || check_textures(cub) == -1)
		return (-1);
	if (cub->v_color->f_num != 1 || cub->v_color->c_num != 1)
		return (-1);
	if (check_color(cub) == -1)
		return (-1);
	return (0);
}

int	handle_line(t_cub *cub, char **splited)
{
	int	i;

	i = 0;
	cub->flag = 0;
	while (splited[i])
		i++;
	parse_texture_line(cub, splited, splited[0]);
	if (parse_color_line(cub, cub->line, splited[0]) == -1)
		return (-1);
	if (cub->flag == 1 && i != 2)
		return (-1);
	return (0);
}
