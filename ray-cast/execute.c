/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:57:28 by her-rehy          #+#    #+#             */
/*   Updated: 2025/03/05 01:50:17 by her-rehy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"
#include <stdlib.h>
#include <string.h>

int	ft_rgb_to_hex(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}
static void	cube_initializing_and_protecting(t_cube *cube, t_cub *cub)
{
	cube->x = 0;
	cube->y = 0;
	cube->ceiling_color = ft_rgb_to_hex(cub->v_color->c_r, cub->v_color->c_g,
			cub->v_color->c_b);
	cube->floor_color = ft_rgb_to_hex(cub->v_color->f_r, cub->v_color->f_g,
			cub->v_color->f_b);
	cube->mlx = mlx_init();
	if (!cube->mlx)
		write(2, "Error: mlx_init\n", 16);
	cube->win = mlx_new_window(cube->mlx, cube->width, cube->height, "cube");
	if (!cube->win)
	{
		mlx_destroy_display(cube->mlx);
		free(cube->mlx);
		write(2, "Error: mlx_new_window\n", 23);
	}
	cube->img = mlx_new_image(cube->mlx, cube->width, cube->height);
	if (!cube->img)
	{
		mlx_destroy_window(cube->mlx, cube->win);
		mlx_destroy_display(cube->mlx);
		free(cube->mlx);
		write(2, "Error: mlx_new_image\n", 21);
	}
	cube->addr = mlx_get_data_addr(cube->img, &cube->bits_per_pixel,
			&cube->line_length, &cube->endian);
	if (!load_textures(cube))
	{
		write(2, "Error: load_textures\n", 22);
		destroy_cube(cube);
	}
	event_init(cube);
	cube_render(cube);
}

void	start_game(t_cube *cube, t_cub *cub)
{
	cube->width = 800;
	cube->height = 600;
	cube->pi = PI;

	cube_initializing_and_protecting(cube, cub);
	
	mlx_loop(cube->mlx);
}