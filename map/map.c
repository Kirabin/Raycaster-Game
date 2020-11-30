/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 10:21:52 by dmilan            #+#    #+#             */
/*   Updated: 2020/11/30 17:57:24 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"



void	parce_resolution(t_vars *vars, const char *line)
{
	int		max_x;
	int		max_y;
	
	line = ft_strskip_char(line, ' ');
	vars->resolution.x = ft_atoi(line);
	line = ft_strskip_char(ft_strchr(line, ' '), ' ');
	vars->resolution.y = ft_atoi(line);
}

// void	free_map(t_map *map)
// {
// 	int		i;

// 	i = 0;
// 	// free(map.texture.sprite);
// 	while (i < map.y)
// 		free(map[i++]);
// 	free(map.x);
// }



int		parce_map_element(t_vars *vars, char *line)
{
	if (*line == 'R')
		parce_resolution(vars, line + 1);
	else if (*line == 'F')
		vars->texture.floor = parce_color(line + 1);
	else if (*line == 'C')
		vars->texture.ceilling = parce_color(line + 1);
	else if (*line == 'S' && *(line + 1) != 'O')
	{
		vars->texture.sprite.image = mlx_xpm_file_to_image(vars->mlx, (char *)ft_strskip_char(line + 1, ' '), &vars->texture.sprite.width, &vars->texture.sprite.height);
	}
	else if (ft_strncmp(line, "NO", 2) == 0)
	{
		vars->texture.north.image = mlx_xpm_file_to_image(vars->mlx, (char *)ft_strskip_char(line + 2, ' '), &vars->texture.north.width, &vars->texture.north.height);
	}
	else if (ft_strncmp(line, "EA", 2) == 0)
	{
		vars->texture.east.image = mlx_xpm_file_to_image(vars->mlx, (char *)ft_strskip_char(line + 2, ' '), &vars->texture.east.width, &vars->texture.east.height);
	}
	else if (ft_strncmp(line, "SO", 2) == 0)
	{
		vars->texture.south.image = mlx_xpm_file_to_image(vars->mlx, (char *)ft_strskip_char(line + 2, ' '), &vars->texture.south.width, &vars->texture.south.height);
	}
	else if (ft_strncmp(line, "WE", 2) == 0)
	{
		vars->texture.west.image = mlx_xpm_file_to_image(vars->mlx, (char *)ft_strskip_char(line + 2, ' '), &vars->texture.west.width, &vars->texture.west.height);
	}
	else if (*line == '\n' || *line == '\0')
	{
		;
	}
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
	int			flag;
	int			result;
	int			fd;
	int			i;
	int			j;
	char		*map_char;

	i = 0;
	flag = 0;
	if ((fd = open(file, O_RDONLY)) == -1)
		return (0);
	while ((result = get_next_line(fd, &line)) != 0)
	{
		if (result == -1)
		{
			// free(map);
			return (0);
		}
		if (!flag && parce_map_element(vars, line))
			continue;
		else
		{
			flag = 1;
			vars->y++;
		}
		free(line);
	}
	free(line);
	if (!(vars->x = malloc(sizeof(int) * vars->y)))
		return (0);
	if (!(vars->map = malloc(sizeof(char*) * vars->y)))
	{
		free(vars->x);
		return (0);
	}
	close(fd);
	fd = open(file, O_RDONLY);
	flag = 0;
	while ((result = get_next_line(fd, &line)) != 0)
	{
		if (result == -1)
		{
			// free(map);
			return (0);
		}
		if (!flag && parce_map_element(vars, line))
			continue;
		else
		{
			flag = 1;
			vars->x[i] = ft_strlen(line);
			if (!(vars->map[i] = ft_strdup(line)))
			{
				free(vars->x);
				// free allocated vars->field;
				return (0);
			}
			j = 0;
			while (j < vars->x[i])  // not effectitve
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
	return (1);  //free_map()
}
