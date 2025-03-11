/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-baz <abel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:57:28 by her-rehy          #+#    #+#             */
/*   Updated: 2025/03/11 00:21:03 by abel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

int	is_wall(float x, float y, t_cube *cube)
{
	float	buffer;

	buffer = 0.2;
	cube->player_px += x / TILE_SIZE;
	cube->player_py += y / TILE_SIZE;
	cube->player_py += 0.7;
	cube->player_px += 0.7;
	if (cube->cub->v_map->map[(int)(cube->player_py
			- buffer)][(int)(cube->player_px - buffer)] == '1'
		|| cube->cub->v_map->map[(int)(cube->player_py
			- buffer)][(int)(cube->player_px - buffer)] == ' ')
		return (1);
	return (0);
}

void	put_pixel_to_image(int x, int y, int color, t_cube *cube)
{
	char	*dst;

	if (x >= 0 && x < cube->width && y >= 0 && y < cube->height)
	{
		dst = cube->addr + (y * cube->line_length + x * (cube->bits_per_pixel
					/ 8));
		*(unsigned int *)dst = color;
	}
}

int	ft_rgb_to_hex(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

static int	cube_init(t_cube *cube, t_cub *cub)
{
	cube->x = 0;
	cube->y = 0;
	cube->ceiling_color = ft_rgb_to_hex(cub->v_color->c_r, cub->v_color->c_g,
			cub->v_color->c_b);
	cube->floor_color = ft_rgb_to_hex(cub->v_color->f_r, cub->v_color->f_g,
			cub->v_color->f_b);
	cube->mlx = mlx_init();
	cube->win = mlx_new_window(cube->mlx, cube->width, cube->height, "cube");
	cube->img = mlx_new_image(cube->mlx, cube->width, cube->height);
	cube->addr = mlx_get_data_addr(cube->img, &cube->bits_per_pixel,
			&cube->line_length, &cube->endian);
	if (!cube->mlx || !cube->win || !cube->img || !load_textures(cube))
		return (write(1, "Error: in initializing cube\n", 29),
			destroy_cube(cube));
	event_init(cube);
	cube_render(cube);
	return (0);
}

void	start_game(t_cube *cube, t_cub *cub)
{
	cube->width = 800;
	cube->height = 600;
	cube_init(cube, cub);
	mlx_loop(cube->mlx);
}
