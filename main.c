#include "cub.h"

void    init_cub(t_cub *cub, char **av)
{
    cub->v_texture = ft_calloc(1, sizeof(t_texture));
    if (!cub->v_texture)
        return ;//clean up function
    cub->v_color = ft_calloc(1, sizeof(t_color));
    if (!cub->v_color)
        return ;//clean up function
    cub->v_map = ft_calloc(1, sizeof(t_map));
    if (!cub->v_map)
        return ;//clean up function
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
        return (1);
    }
    if (parse_cub(av[1], &cub) == -1)
    {
        printf("Error\n parsing failed\n");
        close(cub.fd);//clean up function
        return (1);
    }
    // else
    //     printf("parsing successful\n");

    start_game(&cube, &cub);
    close(cub.fd);//clean up function
    return (0);
}
