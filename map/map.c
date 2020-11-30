/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 10:21:52 by dmilan            #+#    #+#             */
/*   Updated: 2020/11/30 11:27:17 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		argb_color(int a, int r, int g, int b)
{
	return(a << 24 | r << 16 | g << 8 | b);
}

void	parce_resolution(t_map *map, const char *line)
{
	line = ft_strskip_char(line, ' ');
	map->resolution.x = ft_atoi(line);
	line = ft_strskip_char(ft_strchr(line, ' '), ' ');
	map->resolution.y = ft_atoi(line);
}

t_color		parce_color(t_map *map, const char *line)
{
	int		r;
	int		g;
	int		b;
	t_color	color;

	line = ft_strskip_char(line, ' ');
	color.r = ft_atoi(line);
	line = ft_strchr(line, ',') + 1;
	color.g = ft_atoi(line);
	line = ft_strchr(line, ',') + 1;
	color.b = ft_atoi(line);
	color.argb = argb_color(0, color.r, color.g, color.b);
	return (color);
}

void	free_map(t_map *map)
{
	int		i;

	i = 0;
	free(map->texture.sprite);
	while (i < map->y)
		free(map->field[i++]);
	free(map->x);
	free(map);
}

t_map	*default_map(void)
{
	t_map	*map;

	if (!(map = malloc(sizeof(t_map))))
		return (0);
	map->texture.sprite = NULL;
	map->y = 0;
	return (map);
}

int		parce_map_element(t_map *map, char *line)
{
	if (*line == 'R')
		parce_resolution(map, line + 1);
	else if (*line == 'F')
		map->texture.floor = parce_color(map, line + 1);
	else if (*line == 'C')
		map->texture.ceilling = parce_color(map, line + 1);
	else if (*line == 'S' && *(line + 1) != 'O')
		map->texture.sprite = ft_strdup(ft_strskip_char(line + 1, ' '));
	else if (ft_strncmp(line, "NO", 2) == 0)
		map->texture.north = ft_strdup(ft_strskip_char(line + 2, ' '));
	else if (ft_strncmp(line, "EA", 2) == 0)
		map->texture.east = ft_strdup(ft_strskip_char(line + 2, ' '));
	else if (ft_strncmp(line, "SO", 2) == 0)
		map->texture.south = ft_strdup(ft_strskip_char(line + 2, ' '));
	else if (ft_strncmp(line, "WE", 2) == 0)
		map->texture.west = ft_strdup(ft_strskip_char(line + 2, ' '));
	else if (*line == '\n' || *line == '\0')
		;
	else
		return (0);
	return (1);
}

/*
**	returns 1 if execution were successful, 0 otherwise
*/

int		read_map(char *file, t_vars *vars)
{
	char		*line;
	t_map		*map;
	int			flag;
	int			result;
	int			fd;
	int			i;
	int			j;
	int			square_size = 20;

	i = 0;
	flag = 0;
	if ((fd = open(file, O_RDONLY)) == -1)
		return (0);
	if (!(map = default_map()))
		return (0);
	while ((result = get_next_line(fd, &line)) != 0)
	{
		if (result == -1)
		{
			free(map);
			return (0);
		}
		if (!flag && parce_map_element(map, line))
			continue;
		else
		{
			flag = 1;
			map->y++;
		}
		free(line);
	}
	free(line);
	if (!(map->x = malloc(sizeof(int) * map->y)))
		return (0);
	if (!(map->field = malloc(sizeof(char*) * map->y)))
	{
		free(map->x);
		free(map);
		return (0);
	}
	close(fd);
	fd = open(file, O_RDONLY);
	flag = 0;
	while ((result = get_next_line(fd, &line)) != 0)
	{
		if (result == -1)
		{
			free(map);
			return (0);
		}
		if (!flag && parce_map_element(map, line))
			continue;
		else
		{
			flag = 1;
			map->x[i] = ft_strlen(line);
			if (!(map->field[i] = ft_strdup(line)))
			{
				free(map->x);
				// free allocated map.field;
				free(map);
				return (0);
			}
			j = 0;
			while (j < map->x[i])  // not effectitve
			{
				if (line[j] == 'N')
				{
					vars->player.direction.x = 0;
					vars->player.direction.y = -1;
					vars->player.position.x = j + 0.5;
					vars->player.position.y = i + 0.5;
					vars->player.plane.x = -0.66;
					vars->player.plane.y = 0;
				}
				else if (line[j] == 'E')
				{
					vars->player.direction.x = 1;
					vars->player.direction.y = 0;
					vars->player.position.x = j + 0.5;
					vars->player.position.y = i + 0.5;
					vars->player.plane.x = 0;
					vars->player.plane.y = -0.66;
				}
				else if (line[j] == 'W')
				{
					vars->player.direction.x = -1;
					vars->player.direction.y = 0;
					vars->player.position.x = j + 0.5;
					vars->player.position.y = i + 0.5;
					vars->player.plane.x = 0;
					vars->player.plane.y = 0.66;
				}
				else if (line[j] == 'S')
				{
					vars->player.direction.x = 0;
					vars->player.direction.y = 1;
					vars->player.position.x = j + 0.5;
					vars->player.position.y = i + 0.5;
					vars->player.plane.x = 0.66;
					vars->player.plane.y = 0;
				}
				j++;
			}
			i++;
		}
		free(line);
	}
	free(line);
	close(fd);
	vars->map = map;
	return (1);  //free_map()
}