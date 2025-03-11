/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-baz <abel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:49:08 by abel-baz          #+#    #+#             */
/*   Updated: 2025/03/10 23:48:09 by abel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

static void	to_be_continued(t_cub *cub, char **splited, char *type)
{
	if (type[0] == 'W' && type[1] == 'E' && type[2] == '\0')
	{
		cub->v_texture->we_num++;
		if (cub->v_texture->we_num == 1)
			cub->v_texture->we = ft_strdup(splited[1]);
		cub->flag = 1;
	}
	else if (type[0] == 'E' && type[1] == 'A' && type[2] == '\0')
	{
		cub->v_texture->ea_num++;
		if (cub->v_texture->ea_num == 1)
			cub->v_texture->ea = ft_strdup(splited[1]);
		cub->flag = 1;
	}
}

void	parse_texture_line(t_cub *cub, char **splited, char *type)
{
	if (type[0] == 'N' && type[1] == 'O' && type[2] == '\0')
	{
		cub->v_texture->no_num++;
		if (cub->v_texture->no_num == 1)
			cub->v_texture->no = ft_strdup(splited[1]);
		cub->flag = 1;
	}
	else if (type[0] == 'S' && type[1] == 'O' && type[2] == '\0')
	{
		cub->v_texture->so_num++;
		if (cub->v_texture->so_num == 1)
			cub->v_texture->so = ft_strdup(splited[1]);
		cub->flag = 1;
	}
	else
		to_be_continued(cub, splited, type);
}

int	parse_color_line(t_cub *cub, char *line, char *splited)
{
	if (splited[0] == 'F' && splited[1] == '\0')
	{
		cub->v_color->f_num++;
		if (count_sep(line, ',') != 2)
			return (-1);
		if (get_rgb(line, cub) == -1)
			return (-1);
	}
	else if (splited[0] == 'C' && splited[1] == '\0')
	{
		cub->v_color->c_num++;
		if (count_sep(line, ',') != 2)
			return (-1);
		if (get_rgb(line, cub) == -1)
			return (-1);
	}
	return (0);
}

static int	process_texture_lines(t_cub *cub)
{
	char	**splited;

	while (cub->line)
	{
		if (is_map(cub->line, cub))
			break ;
		if (empty_line(cub->line))
		{
			free(cub->line);
			cub->line = NULL;
			cub->line = get_next_line(cub->fd);
			continue ;
		}
		splited = ft_splitt(cub->line, ", \n");
		if (!splited || handle_line(cub, splited) == -1)
		{
			free_splited(splited);
			return (-1);
		}
		free(cub->line);
		cub->line = NULL;
		free_splited(splited);
		cub->line = get_next_line(cub->fd);
	}
	return (0);
}

int	parse_texture_color(t_cub *cub)
{
	if (process_texture_lines(cub) == -1)
	{
		free(cub->line);
		cub->line = NULL;
		return (-1);
	}
	if (holy_checks(cub) == -1)
	{
		free(cub->line);
		cub->line = NULL;
		return (-1);
	}
	return (0);
}
