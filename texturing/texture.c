/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 03:20:44 by her-rehy          #+#    #+#             */
/*   Updated: 2025/03/08 03:42:15 by her-rehy         ###   ########.fr       */
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

int	get_texture_color(t_cube *cube, int tex_x, int tex_y, int side)
{
	int	color;
	int	offset;

	color = 0;
	if (cube->tex_img[side].addr == NULL)
		destroy_cube(cube);
	offset = tex_y * cube->tex_img[side].line_length + tex_x
		* (cube->tex_img[side].bits_per_pixel / 8);
	if (offset < 0 || offset >= cube->tex_img[side].line_length
		* cube->tex_img[side].height)
		destroy_cube(cube);
	color = *(int *)(cube->tex_img[side].addr + offset);
	return (color);
}

void	render_vertical_line(t_cube *cube, t_draw_data *draw, int ray,
		float wall_height)
{
	int	y;

	y = 0;
	while (y < cube->height)
	{
		if (y < draw->start)
			put_pixel_to_image(ray, y, cube->ceiling_color, cube);
		else if (y < draw->end)
		{
			draw->tex_i = (float)(y - draw->start) / wall_height
				* cube->tex_img[draw->side].height;
			draw->tex_y = (int)draw->tex_i;
			if (draw->tex_y < 0)
				draw->tex_y = 0;
			if (draw->tex_y >= cube->tex_img[draw->side].height)
				draw->tex_y = cube->tex_img[draw->side].height - 1;
			draw->tex_color = get_texture_color(cube, draw->tex_x, draw->tex_y,
					draw->side);
			put_pixel_to_image(ray, y, draw->tex_color, cube);
		}
		else
			put_pixel_to_image(ray, y, cube->floor_color, cube);
		y++;
	}
}

void	calculate_draw_data(t_cube *cube, t_distance *d, t_draw_data *draw,
		float hit_x, float hit_y)
{
	if (cube->hit_side == 1)
	{
		draw->tilecoord = hit_x / (float)TILE_SIZE;
		if (d->ray_sin <= 0)
			draw->side = 0;
		else
			draw->side = 1;
	}
	else
	{
		draw->tilecoord = hit_y / (float)TILE_SIZE;
		if (d->ray_cos <= 0)
			draw->side = 2;
		else
			draw->side = 3;
	}
	draw->wall_hit = draw->tilecoord - floor(draw->tilecoord);
	draw->tex_x = (int)(draw->wall_hit
			* (float)cube->tex_img[draw->side].width);
	if (draw->tex_x < 0)
		draw->tex_x = 0;
	else if (draw->tex_x >= cube->tex_img[draw->side].width)
		draw->tex_x = cube->tex_img[draw->side].width - 1;
}

void	draw_vertical_line(t_cube *cube, t_distance *d, int ray,
		float wall_height, int color, float hit_x, float hit_y)
{
	t_draw_data draw;

	draw.start = (cube->height - (int)wall_height) / 2;
	draw.end = (cube->height + (int)wall_height) / 2;
	calculate_draw_data(cube, d, &draw, hit_x, hit_y);
	render_vertical_line(cube, &draw, ray, wall_height);
}