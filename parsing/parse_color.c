#include "../cub.h"

int check_color(t_cub *cub)
{
    if (cub->v_color->f_r < 0 || cub->v_color->f_r > 255)
        return (-1);
    else if (cub->v_color->f_g < 0 || cub->v_color->f_g > 255)
        return (-1);
    else if (cub->v_color->f_b < 0 || cub->v_color->f_b > 255)
        return (-1);
    else if (cub->v_color->c_r < 0 || cub->v_color->c_r > 255)
        return (-1);
    else if (cub->v_color->c_g < 0 || cub->v_color->c_g > 255)
        return (-1);
    else if (cub->v_color->c_b < 0 || cub->v_color->c_b > 255)
        return (-1);
    return (0);
}

int get_rgb(char *line, t_cub *cub)
{
    char    **splited;
    int     i;
    
    splited = ft_splitt(line, ", \n");
    i = 0;
    while (splited[i])
        i++;
    if (i != 4)
        return (-1);
    if (splited[0][0] == 'F' && splited[0][1] == '\0') 
    {
        cub->v_color->f_r = ft_atoi(splited[1]);
        cub->v_color->f_g = ft_atoi(splited[2]);
        cub->v_color->f_b = ft_atoi(splited[3]);
    }
    else if (splited[0][0] == 'C' && splited[0][1] == '\0')
    {
        cub->v_color->c_r = ft_atoi(splited[1]);
        cub->v_color->c_g = ft_atoi(splited[2]);
        cub->v_color->c_b = ft_atoi(splited[3]);
    }
    //free_splited(splited);
    return (0);
}


int parse_color(char *line, t_cub *cub)
{
    int status;

    status = 0;
    if (line != NULL)
    {
        if (empty_line(line))
            free(line);
        if (line[0] == 'F' && line[1] == ' ')
        {
            cub->v_color->f_num++;
            status = get_rgb(line, cub);
            if (status == -1)
                return (-1);
        }
        else if (line[0] == 'C' && line[1] == ' ')
        {
            cub->v_color->c_num++;
            status = get_rgb(line, cub);
            if (status == -1)
                return (-1);
        }
        else
            return (free(line), -1);
        free(line);
    }
    while ((line = get_next_line(cub->fd)) != NULL)
    {
        if (empty_line(line))
        {
            free(line);
            break;
        }
        if (line[0] == 'F' && line[1] == ' ')
        {
            cub->v_color->f_num++;
            status = get_rgb(line, cub);
            if (status == -1)
                return (-1);
        }
        else if (line[0] == 'C' && line[1] == ' ')
        {
            cub->v_color->c_num++;
            status = get_rgb(line, cub);
            if (status == -1)
                return (-1);
        }
        else
            return (free(line), -1);
        free(line);
    }
    if (cub->v_color->f_num != 1 || cub->v_color->c_num != 1)
        return (-1);

    status = check_color(cub);
    return (status);
}
