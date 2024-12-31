#include "../cub.h"

int check_textures(t_cub *cub)
{
    int fd;

    fd = open(cub->v_texture->no, O_RDONLY);
    if (fd == -1)
        return (-1);
    close(fd);
    fd = open(cub->v_texture->so, O_RDONLY);
    if (fd == -1)
        return (-1);
    close(fd);
    fd = open(cub->v_texture->we, O_RDONLY);
    if (fd == -1)
        return (-1);
    close(fd);
    fd = open(cub->v_texture->ea, O_RDONLY);
    if (fd == -1)
        return (-1);
    close(fd);
    return (0);
}

int parse_textures(t_cub *cub)
{
    char *line;
    int state;
    char **splited;
    int i;

    state = 0;
    while ((line = get_next_line(cub->fd)) != NULL)
    {
        i = 0;
        if (empty_line(line))
        {
            free(line);
            break;
        }
        if ((line[0] == 'N' && line[1] == 'O' && line[2] == ' '))
        {
            splited = ft_splitt(line, ", \n");
            while (splited[i])
                i++;
            if (i != 2)
                return (free(line), -1);//clean up function
            cub->v_texture->no_num++;
            cub->v_texture->no = ft_strdup(splited[1]);
        }
        else if ((line[0] == 'S' && line[1] == 'O' && line[2] == ' '))
        {
            splited = ft_splitt(line, ", \n");
            while (splited[i])
                i++;
            if (i != 2)
                return (free(line), -1); //clean up function
            cub->v_texture->so_num++;
            cub->v_texture->so = ft_strdup(splited[1]);
        }
        else if ((line[0] == 'W' && line[1] == 'E' && line[2] == ' '))
        {
            splited = ft_splitt(line, ", \n");
            while (splited[i])
                i++;
            if (i != 2)
                return (free(line), -1); //clean up function
            cub->v_texture->we_num++;
            cub->v_texture->we = ft_strdup(splited[1]);
        }
        else if ((line[0] == 'E' && line[1] == 'A' && line[2] == ' '))
        {
            splited = ft_splitt(line, ", \n");
            while (splited[i])
                i++;
            if (i != 2)
                return (free(line), -1); //clean up function
            cub->v_texture->ea_num++;
            cub->v_texture->ea = ft_strdup(splited[1]);
        }
        else
            return (free(line), -1); //clean up function
        free(line);
    }
    if (cub->v_texture->no_num != 1 || cub->v_texture->so_num != 1 
        || cub->v_texture->we_num != 1 || cub->v_texture->ea_num != 1)
        return (-1); //clean up function
    state = check_textures(cub);
    return (state);
}
