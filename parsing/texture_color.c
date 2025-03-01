#include "../cub.h"


//here we parse the texture line

// int check_splited(char **splited, int num)
// {
//     int i = 0;
//     while (splited[i])
//         i++;
//     if (i != num)
//         return -1;
//     return 0;
// }

int holy_checks(t_cub *cub)
{
    if (check_texture_counts(cub) == -1 || check_textures(cub) == -1)
        return -1;
    if (cub->v_color->f_num != 1 || cub->v_color->c_num != 1)
        return (-1);
    if (check_color(cub) == -1)
        return (-1);
    return 0;
}

void parse_texture_line(t_cub *cub, char **splited, char *type)
{
    if (type[0] == 'N' && type[1] == 'O' && type[2] == '\0')
    {
        cub->v_texture->no_num++;
        if (cub->v_texture->no_num == 1)
            cub->v_texture->no = ft_strdup(splited[1]);
        cub->flag = 1;
    }
    else if (type[0] == 'S' && type[1] == 'O' && type[2] == '\0')
    {
        cub->v_texture->so_num++;
        if (cub->v_texture->so_num == 1)
            cub->v_texture->so = ft_strdup(splited[1]);
        cub->flag = 1;
    }
    else if (type[0] == 'W' && type[1] == 'E' && type[2] == '\0')
    {
        cub->v_texture->we_num++;
        if (cub->v_texture->we_num == 1)
            cub->v_texture->we = ft_strdup(splited[1]);
        cub->flag = 1;
    }
    else if (type[0] == 'E' && type[1] == 'A' && type[2] == '\0')
    {
        cub->v_texture->ea_num++;
        if (cub->v_texture->ea_num == 1)
            cub->v_texture->ea = ft_strdup(splited[1]);
        cub->flag = 1;
    }
}


int parse_color_line(t_cub *cub, char *line, char *splited)
{
        if (splited[0] == 'F' && splited[1] == '\0')
        {
            cub->v_color->f_num++;
            if (count_sep(line, ',') != 2)
                return (-1);
            if (get_rgb(line, cub) == -1)
                return (-1);
            //cub->flag = 2;
        }
        else if (splited[0]== 'C' && splited[1] == '\0')
        {
            cub->v_color->c_num++;
            if (count_sep(line, ',') != 2)
                return (-1);
            if (get_rgb(line, cub) == -1)
                return (-1);
            //cub->flag = 2;
        }
    return (0);
}

static int handle_line(t_cub *cub, char **splited)
{
    int i = 0;
    cub->flag = 0;
    while (splited[i])
        i++;
    parse_texture_line(cub, splited, splited[0]);
    if (parse_color_line(cub, cub->line, splited[0]) == -1)
        return -1;
    if (cub->flag == 1 && i != 2)
        return -1;
    return 0;
}

int parse_texture_color(t_cub *cub)
{
    char **splited;

    while (cub->line)
    {
        if (is_map(cub->line, cub))
            break;
        if (empty_line(cub->line))
        {
            free(cub->line);
            cub->line = NULL;  // Set to NULL after freeing
            cub->line = get_next_line(cub->fd);
            continue;
        }
        splited = ft_splitt(cub->line, ", \n");
        if (!splited || handle_line(cub, splited) == -1)
        {
            free(cub->line);
            cub->line = NULL;  // Set to NULL after freeing
            free_splited(splited);
            return -1;
        }
        free(cub->line);
        cub->line = NULL;  // Set to NULL after freeing
        free_splited(splited);
        cub->line = get_next_line(cub->fd);
    }
    if (holy_checks(cub) == -1)
    {
        free(cub->line);
        cub->line = NULL;  // Set to NULL after freeing
        return -1;
    }
    return 0;
}
