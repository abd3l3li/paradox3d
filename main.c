#include "cub.h"

void free_cub_resources(t_cub *cub)
{
    int i;

    i = 0;
    if (cub->v_texture)
    {
        if (cub->v_texture->no)
            free(cub->v_texture->no);
        if (cub->v_texture->so)
            free(cub->v_texture->so);
        if (cub->v_texture->we)
            free(cub->v_texture->we);
        if (cub->v_texture->ea)
            free(cub->v_texture->ea);
        free(cub->v_texture);
    }
    if (cub->v_color)
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
    if (cub->line)
        free(cub->line);
    if (cub->str)
        free(cub->str);
    drain_gnl(cub->fd);
}

void    init_cub(t_cub *cub, char **av)
{
    cub->v_texture = ft_calloc(1, sizeof(t_texture));
    if (!cub->v_texture)
    {
        free_cub_resources(cub);
        return ;//clean up function
    }
    cub->v_color = ft_calloc(1, sizeof(t_color));
    if (!cub->v_color)
    {
        free_cub_resources(cub);
        return ;//clean up function
    }
    cub->v_map = ft_calloc(1, sizeof(t_map));
    if (!cub->v_map)
    {
        free_cub_resources(cub);
        return ;//clean up function
    }
    cub->v_map->argv = av[1];
    cub->str = NULL;
    cub->fd = 0;
    cub->flag = 0;
    cub->line = NULL;
    cub->v_map->manner = 4;
    cub->v_map->map_len = map_alloc(cub);
    cub->v_map->manner = 4;
}

int main(int ac, char **av)
{
    t_cub   cub;
    t_cube	cube;

    cube.player_angle = 0;
    if (ac != 2)
    {
        printf("Error\n not enough arguments\n");
        return (1);
    } 
    init_cub(&cub, av);
    cub.fd = open(av[1], O_RDONLY);
    if (cub.fd == -1 )
    {
        printf("Error\n file not found\n");
        free_cub_resources(&cub);
        return (1);
    }
    if (parse_cub(av[1], &cub) == -1)
    {
        printf("Error\n parsing failed\n");
        free_cub_resources(&cub);
        close(cub.fd);//clean up function
        return (1);
    }
    // else
    //     printf("parsing successful\n");

    init_texture(&cube, cub.v_texture->no);
    start_game(&cube, &cub);
    
    free_cub_resources(&cub);
    close(cub.fd);//clean up function
    return (0);
}
