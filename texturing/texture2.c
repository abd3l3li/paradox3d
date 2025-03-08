/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 06:30:49 by her-rehy          #+#    #+#             */
/*   Updated: 2025/03/08 06:31:18 by her-rehy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

float	player_facing(t_cube *data)
{
	size_t	i;
	int		j;

	j = 0;
	while (j < data->cub->v_map->map_len)
	{
		i = 0;
		while (i < ft_strlen(data->cub->v_map->map[j]))
		{
			if (data->cub->v_map->map[j][i] == 'N')
				return (1.5 * PI);
			else if (data->cub->v_map->map[j][i] == 'S')
				return (0.5 * PI);
			else if (data->cub->v_map->map[j][i] == 'E')
				return (0);
			else if (data->cub->v_map->map[j][i] == 'W')
				return (PI);
			i++;
		}
		j++;
	}
	return (0);
}

int	get_texture_addresses(t_cube *cube)
{
	cube->tex_img[0].addr = mlx_get_data_addr(cube->tex_img[0].img,
			&cube->tex_img[0].bits_per_pixel, &cube->tex_img[0].line_length,
			&cube->tex_img[0].endian);
	cube->tex_img[1].addr = mlx_get_data_addr(cube->tex_img[1].img,
			&cube->tex_img[1].bits_per_pixel, &cube->tex_img[1].line_length,
			&cube->tex_img[1].endian);
	cube->tex_img[2].addr = mlx_get_data_addr(cube->tex_img[2].img,
			&cube->tex_img[2].bits_per_pixel, &cube->tex_img[2].line_length,
			&cube->tex_img[2].endian);
	cube->tex_img[3].addr = mlx_get_data_addr(cube->tex_img[3].img,
			&cube->tex_img[3].bits_per_pixel, &cube->tex_img[3].line_length,
			&cube->tex_img[3].endian);
	return (1);
}

int	load_textures(t_cube *cube)
{
	cube->tex_img[0].img = mlx_xpm_file_to_image(cube->mlx,
			cube->cub->v_texture->no, &cube->tex_img[0].width,
			&cube->tex_img[0].height);
	cube->tex_img[1].img = mlx_xpm_file_to_image(cube->mlx,
			cube->cub->v_texture->so, &cube->tex_img[1].width,
			&cube->tex_img[1].height);
	cube->tex_img[2].img = mlx_xpm_file_to_image(cube->mlx,
			cube->cub->v_texture->we, &cube->tex_img[2].width,
			&cube->tex_img[2].height);
	cube->tex_img[3].img = mlx_xpm_file_to_image(cube->mlx,
			cube->cub->v_texture->ea, &cube->tex_img[3].width,
			&cube->tex_img[3].height);
	if (cube->tex_img[0].img == NULL || cube->tex_img[1].img == NULL
		|| cube->tex_img[2].img == NULL || cube->tex_img[3].img == NULL)
		return (0);
	if (!get_texture_addresses(cube))
		return (0);
	return (1);
}
