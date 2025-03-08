/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:57:28 by her-rehy          #+#    #+#             */
/*   Updated: 2025/03/08 02:15:18 by her-rehy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"
#include <stdlib.h>
#include <string.h>

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
	cube->pi = PI;
	cube_init(cube, cub);
	mlx_loop(cube->mlx);
}
