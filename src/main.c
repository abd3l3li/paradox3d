/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-baz <abel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:48:17 by abel-baz          #+#    #+#             */
/*   Updated: 2025/03/11 00:17:09 by abel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int	destroy_cube(void *param)
{
	int		i;
	t_cube	*cube;

	i = 0;
	cube = (t_cube *)param;
	if (cube->mlx)
	{
		if (cube->img)
			mlx_destroy_image(cube->mlx, cube->img);
		while (i < 4)
		{
			if (cube->tex_img[i].img)
				mlx_destroy_image(cube->mlx, cube->tex_img[i].img);
			i++;
		}
		if (cube->win)
			mlx_destroy_window(cube->mlx, cube->win);
		mlx_destroy_display(cube->mlx);
		free(cube->mlx);
	}
	free_cub_resources(cube->cub, cube);
	exit(0);
}

void	free_cub_io(t_cub *cub)
{
	free(cub->line);
	free(cub->str);
	drain_gnl(cub->fd);
	if (cub->fd > 0)
		close(cub->fd);
}

static void	free_cub_core(t_cub *cub)
{
	int	i;

	i = 0;
	if (cub->v_texture)
	{
		free(cub->v_texture->no);
		free(cub->v_texture->so);
		free(cub->v_texture->we);
		free(cub->v_texture->ea);
		free(cub->v_texture);
	}
	free(cub->v_color);
	if (cub->v_map)
	{
		if (cub->v_map->map)
		{
			while (cub->v_map->map[i])
			{
				free(cub->v_map->map[i]);
				i++;
			}
			free(cub->v_map->map);
		}
		free(cub->v_map);
	}
}

void	free_cub_resources(t_cub *cub, t_cube *cube)
{
	(void)cube;
	free_cub_core(cub);
	free(cub->line);
	free(cub->str);
	drain_gnl(cub->fd);
	if (cub->fd > 0)
		close(cub->fd);
}

int	main(int ac, char **av)
{
	t_cub	cub;
	t_cube	cube;

	if (ac != 2)
	{
		printf("\033[1;31m\033[40m Error\n not enough arguments \033[0m\n");
		return (1);
	}
	init_cub(&cub, av, &cube);
	cub.fd = open(av[1], O_RDONLY);
	if (cub.fd == -1)
	{
		printf("\033[1;31m\033[40m Error\n file not found \033[0m\n");
		free_cub_resources(&cub, &cube);
		return (1);
	}
	if (parse_cub(av[1], &cub) == -1)
	{
		printf("\033[1;31m\033[40m Error\n parsing failed! \033[0m\n");
		free_cub_resources(&cub, &cube);
		return (1);
	}
	start_game(&cube, &cub);
	free_cub_resources(&cub, &cube);
	return (0);
}
