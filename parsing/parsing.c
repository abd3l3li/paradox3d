#include "../cub.h"

int	empty_line(char *line)
{
	int i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ')
		{
			if (line[i] == '\n')
				return (1);
			else
				return (0);
		}
		i++;
	}
	return (1);
}

int	extension_check(char *file)
{
	int i;

	i = 0;
	while (file[i])
		i++;
	if (file[i - 1] != 'b' || file[i - 2] != 'u' 
		|| file[i - 3] != 'c' || file[i - 4] != '.')
		return (-1);
	return (0);
}

int parse_cub(char *file, t_cub *cub)
{
	char *line;

	if (extension_check(file) == -1)
		return (-1);
	if (parse_textures(cub) == -1)
		return (-1);
	while ((line = get_next_line(cub->fd)) != NULL)
	{
		if(!empty_line(line))
			break;
	}
	if (parse_color(line, cub) == -1)
		return (-1);
	while ((line = get_next_line(cub->fd)) != NULL)
	{
		if(!empty_line(line))
			break;
	}
	if (parse_map(line, cub) == -1)
		return (-1);
	while ((line = get_next_line(cub->fd)) != NULL)
	{
		if(!empty_line(line))
			return (-1);
	}
    return (0);
}
