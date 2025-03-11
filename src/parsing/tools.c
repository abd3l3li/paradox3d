/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-baz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:49:27 by abel-baz          #+#    #+#             */
/*   Updated: 2025/03/10 22:49:36 by abel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

int	count_sep(char *line, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		if (line[i] == c)
			count++;
		i++;
	}
	return (count);
}

int	check_color(t_cub *cub)
{
	if (cub->v_color->f_r < 0 || cub->v_color->f_r > 255)
		return (-1);
	else if (cub->v_color->f_g < 0 || cub->v_color->f_g > 255)
		return (-1);
	else if (cub->v_color->f_b < 0 || cub->v_color->f_b > 255)
		return (-1);
	else if (cub->v_color->c_r < 0 || cub->v_color->c_r > 255)
		return (-1);
	else if (cub->v_color->c_g < 0 || cub->v_color->c_g > 255)
		return (-1);
	else if (cub->v_color->c_b < 0 || cub->v_color->c_b > 255)
		return (-1);
	return (0);
}

int	get_rgb(char *line, t_cub *cub)
{
	char	**splited;
	int		i;

	splited = ft_splitt(line, ", \n");
	i = 0;
	while (splited[i])
		i++;
	if (i != 4)
		return (-1);
	if (splited[0][0] == 'F' && splited[0][1] == '\0')
	{
		cub->v_color->f_r = ft_atoi(splited[1]);
		cub->v_color->f_g = ft_atoi(splited[2]);
		cub->v_color->f_b = ft_atoi(splited[3]);
	}
	else if (splited[0][0] == 'C' && splited[0][1] == '\0')
	{
		cub->v_color->c_r = ft_atoi(splited[1]);
		cub->v_color->c_g = ft_atoi(splited[2]);
		cub->v_color->c_b = ft_atoi(splited[3]);
	}
	free_splited(splited);
	return (0);
}

int	check_texture_counts(t_cub *cub)
{
	if (cub->v_texture->no_num != 1 || cub->v_texture->so_num != 1
		|| cub->v_texture->we_num != 1 || cub->v_texture->ea_num != 1)
		return (-1);
	return (0);
}

int	check_textures(t_cub *cub)
{
	int	fd;

	fd = open(cub->v_texture->no, O_RDONLY);
	if (fd == -1)
		return (-1);
	close(fd);
	fd = open(cub->v_texture->so, O_RDONLY);
	if (fd == -1)
		return (-1);
	close(fd);
	fd = open(cub->v_texture->we, O_RDONLY);
	if (fd == -1)
		return (-1);
	close(fd);
	fd = open(cub->v_texture->ea, O_RDONLY);
	if (fd == -1)
		return (-1);
	close(fd);
	return (0);
}
