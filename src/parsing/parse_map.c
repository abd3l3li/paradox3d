/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-baz <abel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:48:42 by abel-baz          #+#    #+#             */
/*   Updated: 2025/03/11 00:13:49 by abel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

int	is_out_of_bounds(t_cub *cub, int i, int j)
{
	if (i == 0 || j == 0 || cub->v_map->map[i + 1] == NULL
		|| cub->v_map->map[i][j + 1] == '\0'
		|| j >= (int)ft_strlen(cub->v_map->map[i - 1])
		|| j >= (int)ft_strlen(cub->v_map->map[i + 1]))
		return (-1);
	return (0);
}

int	not_flood_fill(t_cub *cub)
{
	int	i;
	int	j;

	i = 0;
	while (cub->v_map->map[i])
	{
		j = 0;
		while (cub->v_map->map[i][j])
		{
			if (cub->v_map->map[i][j] == '0' || cub->v_map->map[i][j] == 'W'
				|| cub->v_map->map[i][j] == 'E' || cub->v_map->map[i][j] == 'S'
				|| cub->v_map->map[i][j] == 'N')
			{
				if (is_out_of_bounds(cub, i, j) == -1 || cub->v_map->map[i
						- 1][j] == ' ' || cub->v_map->map[i + 1][j] == ' '
					|| cub->v_map->map[i][j - 1] == ' ' || cub->v_map->map[i][j
					+ 1] == ' ')
					return (-1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	check_elems(t_cub *cub)
{
	int	i;
	int	j;

	i = 0;
	while (cub->v_map->map[i])
	{
		j = 0;
		while (cub->v_map->map[i][j])
		{
			if (cub->v_map->map[i][j] != '1' && cub->v_map->map[i][j] != '0'
				&& cub->v_map->map[i][j] != 'N' && cub->v_map->map[i][j] != 'S'
				&& cub->v_map->map[i][j] != 'E' && cub->v_map->map[i][j] != 'W'
				&& cub->v_map->map[i][j] != ' ')
				return (-1);
			if (cub->v_map->map[i][j] == 'N' || cub->v_map->map[i][j] == 'S'
				|| cub->v_map->map[i][j] == 'E' || cub->v_map->map[i][j] == 'W')
				cub->v_map->player++;
			j++;
		}
		i++;
	}
	if (cub->v_map->player != 1)
		return (-1);
	return (0);
}

int	ft_fill_map(t_cub *cub)
{
	int	i;

	i = 0;
	if (cub->v_map->map_len == -1)
		return (-1);
	if (cub->v_map->map == NULL)
		return (-1);
	while (i < cub->v_map->map_len)
	{
		if (!cub->line)
			return (0);
		cub->v_map->map[i] = ft_strtrim(cub->line, "\n");
		free(cub->line);
		cub->line = get_next_line(cub->fd);
		i++;
	}
	cub->v_map->map[i] = NULL;
	return (0);
}

int	parse_map(t_cub *cub)
{
	int	state;

	state = 0;
	state = ft_fill_map(cub);
	if (state != -1)
		state = check_elems(cub);
	if (state != -1)
		state = not_flood_fill(cub);
	return (state);
}
