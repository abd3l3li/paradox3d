/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-baz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:50:19 by abel-baz          #+#    #+#             */
/*   Updated: 2025/03/10 22:50:28 by abel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

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
		t_render *rend)
{
	if (cube->hit_side == 1)
	{
		draw->tilecoord = rend->hit_x / (float)TILE_SIZE;
		if (d->ray_sin <= 0)
			draw->side = 0;
		else
			draw->side = 1;
	}
	else
	{
		draw->tilecoord = rend->hit_y / (float)TILE_SIZE;
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

void	draw_vertical_line(t_cube *cube, t_distance *d, t_render *rend)
{
	t_draw_data	draw;

	draw.start = (cube->height - (int)rend->wall_height) / 2;
	draw.end = (cube->height + (int)rend->wall_height) / 2;
	calculate_draw_data(cube, d, &draw, rend);
	render_vertical_line(cube, &draw, rend->ray, rend->wall_height);
}
