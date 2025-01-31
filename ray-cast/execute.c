/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:57:28 by her-rehy          #+#    #+#             */
/*   Updated: 2025/01/31 22:31:01 by her-rehy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub_pars.h"

static void	cube_initializing_and_protecting(t_cube *cube)
{
	cube->x = 200;
	cube->y = 300;
	cube->mlx = mlx_init();
	if (!cube->mlx)
		write(2, "Error: mlx_init\n", 16);
	cube->win = mlx_new_window(cube->mlx, 800, 600, "cube");
	if (!cube->win)
	{
		mlx_destroy_display(cube->mlx);
		free(cube->mlx);
		write(2, "Error: mlx_new_window\n", 23);
	}
	cube->img = mlx_new_image(cube->mlx, 800, 600);
	if (!cube->img)
	{
		mlx_destroy_window(cube->mlx, cube->win);
		mlx_destroy_display(cube->mlx);
		free(cube->mlx);
		write(2, "Error: mlx_new_image\n", 21);
	}
	cube->addr = mlx_get_data_addr(cube->img, &cube->bits_per_pixel,
			&cube->line_length, &cube->endian);
	event_init(cube);
	cube_render(cube);
}

void	start_game(t_cube *cube, t_cub *cub)
{
	cube->width = 800;
	cube->height = 600;
	cube_initializing_and_protecting(cube);
	mlx_loop(cube->mlx);
}
