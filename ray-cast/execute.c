/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:57:28 by her-rehy          #+#    #+#             */
/*   Updated: 2025/02/18 01:42:50 by her-rehy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"
#include <stdlib.h>
#include <string.h>

#include <stdlib.h>
#include <string.h>

char** adjust_strings(char **arr)
{
    if (arr == NULL) {
        return NULL;
    }

    int max_len = 0;
    int j = 0;
    char **temp = arr;

    while (*temp != NULL) {
        int len = strlen(*temp);
        if (len > max_len) {
            max_len = len;
        }
        j++;
        temp++;
    }

    char **current = malloc(sizeof(char*) * (j + 1));
    if (!current) {
        return NULL;
    }

    int i = 0;
    while (i < j) {
        current[i] = malloc(sizeof(char) * (max_len + 1));
        if (!current[i]) {
            int k = 0;
            while (k < i) {
                free(current[k]);
                k++;
            }
            free(current);
            return NULL;
        }
        i++;
    }
    current[j] = NULL;

    i = 0;
    while (i < j) {
        int original_len = strlen(arr[i]);
        strcpy(current[i], arr[i]);

        int k = 0;
        while (k < original_len) {
            if (current[i][k] != '0' && current[i][k] != '1' && current[i][k] != 'W') {
                current[i][k] = '1';
            }
            k++;
        }

        k = original_len;
        while (k < max_len) {
            current[i][k] = '1';
            k++;
        }

        current[i][max_len] = '\0';
        i++;
    }

    return current;
}

static void	cube_initializing_and_protecting(t_cube *cube, t_cub *cub)
{
	cube->x = 1;
	cube->y = 1;
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
	int i = 0;

	
	char ** map = adjust_strings(cub->v_map->map);
	while(cub->v_map->map[i])
	{
		free(cub->v_map->map[i]);
		i++;
	}
	i = 0;
	free(cub->v_map->map);
	while(map[i])
	{
		printf("%s\n", map[i]);
		i++;
	}
	cube->map_height = i;
	cube->map_width = strlen(map[0]);
	cub->v_map->map = map;
	cube->cub = cub;
	cube_initializing_and_protecting(cube,cub);
	mlx_loop(cube->mlx);
}
